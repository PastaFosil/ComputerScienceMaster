import pandas as pd
import seaborn as sns
import numpy as np
import search_results_lib as srl
import json
from collections import Counter, defaultdict
from sklearn.metrics import accuracy_score, f1_score

from sklearn.pipeline import Pipeline
from sklearn.feature_selection import SelectKBest, mutual_info_classif
from sklearn.base import clone
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.model_selection import StratifiedKFold, cross_validate, GridSearchCV
from sklearn.metrics import f1_score, accuracy_score
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from collections import defaultdict
import warnings
warnings.filterwarnings("ignore")



def get_feature_selection_pipeline(params):
    pipe = Pipeline([
        ('select', params.get('select')),
        ('scale', params.get('scaler', StandardScaler())),
        ('clf', params.get('clf', LogisticRegression(max_iter=5000)))
    ])
    return pipe

def get_pipeline(model, scaler=None,  select=None, n_components=None, clf=None):
    steps = []
    if scaler is not None:
        steps.append(('scaler', scaler))
    if select is not None:
        if select == 'pca' and n_components is not None:
            steps.append(('select', PCA(n_components=n_components)))
        else:
            steps.append(('select', select))
    steps.append(('model', model))
    return Pipeline(steps)

def evaluate_models(X, y, models, cv, task='regression', scaler=StandardScaler(), select=None, n_components=None, dataset_name='dataset'):
    """
    Evalúa múltiples modelos con CV y devuelve un DataFrame con resultados.
    
    Parámetros
    ----------
    X : array-like o DataFrame
    y : array-like
    models : dict(nombre -> estimador sklearn)
    cv : objeto de validación cruzada
    task : {'regression', 'classification'}
    scaler : transformador o None
    select : None, 'pca' o un selector sklearn
    n_components : int o float, solo si select='pca'
    dataset_name : str, etiqueta del dataset/fuente

    Devuelve
    --------
    df_results : pd.DataFrame
    """
    rows = []

    if task == "regression":
        scoring = {
            "r2": "r2",
            "mae": "neg_mean_absolute_error",
            "mse": "neg_mean_squared_error",
            "rmse": "neg_root_mean_squared_error",
        }
    elif task == "classification":
        scoring = {
            "accuracy": "accuracy",
            "f1_macro": "f1_macro",
            "precision_macro": "precision_macro",
            "recall_macro": "recall_macro",
        }
    else:
        raise ValueError("task debe ser 'regression' o 'classification'.")

    CLASS_MODELS = ['LR', 'SVC_lineal', 'SVC_rbf']
    
    for name, model in models.items():
        # filtrar por tipo de tarea
        if task == "regression" and name in CLASS_MODELS:
            continue
        if task == "classification" and name not in CLASS_MODELS:
            continue

        pipe = get_pipeline(
            model=clone(model),
            scaler=scaler,
            select=select,
            n_components=n_components
        )

        cv_out = cross_validate(
            pipe,
            X,
            y,
            cv=cv,
            scoring=scoring,
            return_train_score=False,
            n_jobs=-1,
            error_score=np.nan
        )

        row = {
            "dataset": dataset_name,
            "task": task,
            "model": name,
            "selector": "none" if select is None else ("pca" if select == "pca" else type(select).__name__),
            "n_components": n_components if select == "pca" else np.nan,
            "n_samples": len(y),
            "n_features": X.shape[1],
            "n_folds": len(cv_out["fit_time"]),
        }

        for metric_name, values in cv_out.items():
            if metric_name.startswith("test_"):
                clean_name = metric_name.replace("test_", "")

                vals = np.array(values, dtype=float)

                # revertir signo de métricas "neg_*"
                if clean_name in {"mae", "mse", "rmse"}:
                    vals = -vals

                row[f"{clean_name}_mean"] = np.nanmean(vals)
                row[f"{clean_name}_std"] = np.nanstd(vals)
                row[f"{clean_name}_all"] = list(vals)

        rows.append(row)

    df_results = pd.DataFrame(rows)
    return df_results

countries = [
    "Argentina",
    "Bolivia",
    "Chile",
    "Colombia",
    "Costa Rica",
    "Cuba",
    "Ecuador",
    "El Salvador",
    "Guatemala",
    "Honduras",
    "Mexico",
    "Nicaragua",
    "Panama",
    "Paraguay",
    "Peru",
    #"Puerto Rico",
    "Dominican Republic",
    "Uruguay",
    "Venezuela",
    "Spain"
]
n_countries = len(countries)
print(n_countries)  # 20
countries.sort()
df_dict = {}
corr_dict = {}

path = "./CPI2024-Results-and-trends.csv"
df_index = pd.read_csv(path, header=2, encoding="ISO-8859-1", sep='\t')
df_index = df_index[df_index["Country / Territory"].isin(countries)]
df_index.dropna(how="all", axis=1, inplace=True)
df_index.rename(columns={"Country / Territory": "country"}, inplace=True)
df_index.set_index('country', inplace=True)

cpi2024_sources = {
    "rank": "Rank",
    "cpi": "CPI 2024 score",
    "gi": "S&P / Global Insights Country Risk Ratings",
    "prs": "PRS International Country Risk Guide",
    "vdem": "Varieties of Democracy Project",
    #"wef": "World Economic Forum EOS",
    "wjp": "World Justice Project Rule of Law cpi",
    "bti": "Bertelsmann Foundation Transformation cpi",
    "eiu": "Economist Intelligence Unit Country Ratings"
}
df_index.rename(columns=dict(zip(cpi2024_sources.values(), cpi2024_sources.keys())), inplace=True)

df_index.drop(columns=[i for i in df_index.columns if i not in cpi2024_sources.keys()], inplace=True)
df_index = df_index.reindex(countries)
df_index.dropna(how='all', inplace=True)
df_index.sort_index(inplace=True)
df_dict["cpi"] = df_index

# Leer el archivo JSON guardado
Nombre_Archivo = f"./resultados_corrupcion.json"

with open(Nombre_Archivo, "r") as file:
    resultados = json.load(file)
for i in range(len(resultados)):
    if resultados[i]['country'] == 'Puerto Rico':
        resultados.pop(i)
        break
resultados.sort(key=lambda x: x['country'])

# Leer el archivo JSON guardado
Nombre_Archivo = f"../buscador/resultados/res_unicos_corrupcion.json"

with open(Nombre_Archivo, "r") as file:
    all_hrefs = json.load(file)

max_len = max([len(r['search_results']['results']) for r in resultados])
rankings = {}
for r in resultados:
    pages = [all_hrefs.index(res['href']) for res in r['search_results']['results']]
    if len(pages) < max_len:
        diff = max_len - len(pages)
        pages += [np.nan] * diff
    rankings[r['country']] = pages
df_rankings = pd.DataFrame(rankings).T
df_rankings.sort_index(inplace=True)
df_dict["rankings"] = df_rankings

df_responses = pd.read_csv("./responses_llama.csv", sep='\t', header=0)

multi_choice = ['proposito',
                'informacion_presentada',
                'consecuencias',
                'intencion',
                'localidad_problema',
                'involucrados',
                'emociones']

possible_answers = {"tipo_entidad": ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'],
                    "origen_entidad": ['a', 'b'],
                    "vision": ['a', 'b', 'c', 'd'],
                    'que_muestra': ['a', 'b', 'c', 'd'],
                    'formalidad': ['a', 'b'],
                    'nivel_educacion': ['a', 'b'],
                    'tono': ['a', 'b'],
                    "proposito": ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'],
                    "informacion_presentada": ['a', 'b', 'c'],
                    "consecuencias": ['a', 'b', 'c', 'd'],
                    "intencion": ['a', 'b', 'c', 'd', 'e', 'f'],
                    "localidad_problema": ['a', 'b', 'c'],
                    "involucrados": ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'],
                    "emociones": ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
                    }
single_questions = [q for q in possible_answers if q not in multi_choice]

# humano < 75% precision llama < 0.6
#bad_questions = ['proposito__d', 'tono', 'intencion__c', 'que_muestra', 'proposito__g', 'consecuencias__c', 'proposito__b', 'emociones__h', 'formalidad', 'localidad_problema__a']
bad_questions = ['proposito__d', 'tono', 'que_muestra', 'proposito__g', 'proposito__b', 'emociones__h', 'formalidad']
# humano < 75% precision llama <= 0.6
#bad_questions = ['proposito__d', 'tono', 'intencion__c', 'que_muestra', 'proposito__g', 'consecuencias__c', 'proposito__b', 'involucrados__f', 'origen_entidad', 'intencion__b', 'emociones__h', 'vision', 'intencion__a', 'formalidad', 'involucrados__b', 'tipo_entidad', 'localidad_problema__a', 'localidad_problema__b']
#bad_questions.remove('emociones__h') # se elimina emocion neutral
for q in bad_questions:
    if "__" not in q:
        for i in possible_answers.get(q, []):
            bad_questions.append(f"{q}__{i}")
bad_questions = [bad for bad in bad_questions if "__" in bad]

df_pag_info = srl.analyze_pages(df_responses, multi_choice, possible_answers)
df_pag_info = df_pag_info.drop(columns=bad_questions)
df_pag_info.index.name = 'page_id'
one_hot_cols = [col for col in df_pag_info.columns if df_pag_info[col].isin([0, 1]).all()]
df_pag_info_ordinal = srl.analyze_pages(df_responses, multi_choice, possible_answers, ordinal=True)
df_pag_info_ordinal = df_pag_info_ordinal.drop(columns=bad_questions)
df_dict["cuestionario"] = df_pag_info#[forms_best_feats]
#df_dict["cuestionario_ordinal"] = df_pag_info_ordinal

bad_labels = ['emotion__others', 'sentiment__NEU', 'irony__not ironic']
df_robertuito = pd.read_csv("./robertuito_all_results.csv").drop(columns=bad_labels)
df_robertuito.set_index('pagina', inplace=True)
df_robertuito.index.name = 'page_id'
df_dict["robertuito"] = df_robertuito#[robertuito_best_feats]

df_dict['bow'] = pd.read_csv('./matriz_tfidf.csv', index_col=0)
df_dict['bow'].index.name = 'page_id'
#bow_cols_valid = [col for col in bow_best_feats if col in df_dict['bow'].columns]
df_dict['bow'] = df_dict['bow']#[bow_cols_valid]

df_dict['regresion'] = df_dict['cpi'][['cpi']]
dict_class_names = {
    2: ['alta', 'baja'],
    3: ['alta', 'media', 'baja'],
    4: ['muy alta', 'alta', 'baja', 'muy baja']
}
for i in [2, 3, 4]:
    df_dict['regresion'][f'{i}-clases'] = pd.qcut(df_dict['cpi']['cpi'], q=i, labels=dict_class_names[i])
sns.barplot(x=df_dict['regresion']['3-clases'], y=df_dict['cpi']['cpi'], order=dict_class_names[3])

cv_outer = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)
cv_inner = StratifiedKFold(n_splits=3, shuffle=True, random_state=42)

models = {
    "logreg": {
        "select": SelectKBest(score_func=mutual_info_classif),
        "scaler": StandardScaler(),
        "clf": LogisticRegression(max_iter=5000)
    },
    'svc': {
        "select": SelectKBest(score_func=mutual_info_classif),
        "scaler": StandardScaler(),
        "clf": SVC()
    }
}

param_grids = {
    "logreg": {
        "select__k": [20],#[5, 10, 20, 50, "all"],
        "clf__C": [0.01, 0.1, 1, 10],
    },
    "svc": {
        "select__k": [20],#[5, 10, 20, 50, "all"],
        "clf__C": [0.01, 0.1, 1, 10],
        "clf__kernel": ["linear", "rbf"],
        "clf__gamma": ["scale", "auto"],
    },
}

segmentation = 10

# FEATURES forms
_, paises_forms = srl.compute_rank_feature_distance(
    df_dict["rankings"],
    df_dict["cuestionario"],
    segment_size=segmentation,
    expanded=True,
    #seg_feat_dict=forms_seg_feat_dict,
    by="no_filter",
    threshold=0.6,
    compare_to=df_dict["regresion"][["cpi"]]
)

#X_forms = paises_forms.values

# FEATURES robertuito
_, paises_robertuito = srl.compute_rank_feature_distance(
    df_dict["rankings"],
    df_dict["robertuito"],
    segment_size=segmentation,
    expanded=True,
    #seg_feat_dict=robertuito_seg_feat_dict
)

#X_robertuito = paises_robertuito.values

# FEATURES bow
_, paises_bow = srl.compute_rank_feature_distance(
    df_dict["rankings"],
    df_dict["bow"],
    segment_size=segmentation,
    expanded=True,
    #seg_feat_dict=bow_seg_feat_dict,
    by="no_filter",
    threshold=0.6,
    compare_to=df_dict["regresion"][["cpi"]]
)

#X_bow = paises_bow.values

feature_sets = {
    "forms": paises_forms,
    "robertuito": paises_robertuito,
    "bow": paises_bow,
}

results = []
selected_features_rows = []
selection_frequency = defaultdict(Counter)
scores_rows = []

for target_name in df_dict['regresion'].columns[1:]:
    print(f"\n===== Target: {target_name} =====")
    
    y_series = df_dict['regresion'][target_name]

    for dataset_name, X_df in feature_sets.items():
        print(f"\n--- Dataset: {dataset_name} ---")

        # Alinear índices y quitar NaN en y
        common_idx = X_df.index.intersection(y_series.dropna().index)
        X_df_aligned = X_df.loc[common_idx]
        y_aligned = y_series.loc[common_idx]

        X = X_df_aligned.values
        y = y_aligned.values
        feature_names = X_df_aligned.columns.to_numpy()

        valid_k = [k for k in [5, 10, 20, 50] if k <= X.shape[1]] + ["all"]

        for model_name in models.keys():
            print(f"Model: {model_name}")

            model_param_grid = param_grids[model_name].copy()
            model_param_grid["select__k"] = valid_k

            fold_accuracies = []
            fold_f1s = []

            for fold_id, (train_idx, test_idx) in enumerate(cv_outer.split(X, y), start=1):
                X_train, X_test = X[train_idx], X[test_idx]
                y_train, y_test = y[train_idx], y[test_idx]

                search = GridSearchCV(
                    estimator=get_feature_selection_pipeline(models[model_name]),
                    param_grid=model_param_grid,
                    cv=cv_inner,
                    scoring="f1_macro",
                    n_jobs=-1
                )

                search.fit(X_train, y_train)

                best_pipe = search.best_estimator_
                y_pred = best_pipe.predict(X_test)

                acc = accuracy_score(y_test, y_pred)
                f1 = f1_score(y_test, y_pred, average="macro")

                fold_accuracies.append(acc)
                fold_f1s.append(f1)

                # Features seleccionadas en este fold
                selector = best_pipe.named_steps["select"]
                mask = selector.get_support()
                selected_features = feature_names[mask]

                # Guardar lista por fold
                selected_features_rows.append({
                    "target": target_name,
                    "dataset": dataset_name,
                    "model": model_name,
                    "fold": fold_id,
                    "n_selected": len(selected_features),
                    "selected_features": list(selected_features),
                    "best_params": search.best_params_
                })

                for feat, score, is_selected in zip(feature_names, selector.scores_, selector.get_support()):
                    scores_rows.append({
                        "target": target_name,
                        "dataset": dataset_name,
                        "model": model_name,
                        "fold": fold_id,
                        "feature": feat,
                        "score": score,
                        "selected": bool(is_selected)
                    })

                # Contar frecuencia
                key = (target_name, dataset_name, model_name)
                selection_frequency[key].update(selected_features)

            # Resumen de métricas para esta combinación
            results.append({
                "target": target_name,
                "dataset": dataset_name,
                "model": model_name,
                "n_samples": len(y),
                "n_features": X.shape[1],
                "accuracy_mean": np.mean(fold_accuracies),
                "accuracy_std": np.std(fold_accuracies),
                "f1_macro_mean": np.mean(fold_f1s),
                "f1_macro_std": np.std(fold_f1s),
            })

results_df = pd.DataFrame(results).sort_values(
    ['target', 'f1_macro_mean'],
    ascending=[True, False]
)

selected_features_df = pd.DataFrame(selected_features_rows)

feature_scores_df = pd.DataFrame(scores_rows)

freq_rows = []

for (target_name, dataset_name, model_name), counter in selection_frequency.items():
    total_folds = cv_outer.get_n_splits()
    for feat, count in counter.items():
        freq_rows.append({
            "target": target_name,
            "dataset": dataset_name,
            "model": model_name,
            "feature": feat,
            "times_selected": count,
            "selection_rate": count / total_folds
        })

selection_freq_df = pd.DataFrame(freq_rows).sort_values(
    ["target", "dataset", "model", "selection_rate", "times_selected"],
    ascending=[True, True, True, False, False]
)

selected_features_df_to_save = selected_features_df.copy()
selected_features_df_to_save["selected_features"] = selected_features_df_to_save["selected_features"].apply(
    lambda x: " | ".join(x)
)
selected_features_df_to_save["best_params"] = selected_features_df_to_save["best_params"].astype(str)

out_dir = "./feature_selection_results/"
results_df.to_csv(out_dir + "clf_results_summary.csv", index=False)
selected_features_df_to_save.to_csv(out_dir + "clf_selected_features_by_fold.csv", index=False)
selection_freq_df.to_csv(out_dir + "clf_feature_selection_frequency.csv", index=False)
feature_scores_df.to_csv(out_dir + "clf_feature_scores_by_fold.csv", index=False)