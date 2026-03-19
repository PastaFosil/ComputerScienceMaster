import numpy as np
import pandas as pd
import math
from scipy.stats import kendalltau, spearmanr, pearsonr
from statsmodels.stats.multitest import multipletests
import matplotlib.pyplot as plt

# ------------------------------------------------
# ANALISIS SOBRE RANKINGS DE PAISES
# ------------------------------------------------

def correlation(data):
    '''
        Calcula la correlacion entre las columnas de un DataFrame.
    '''
    sigma = data.std(axis=0)
    c = np.zeros((data.shape[1], data.shape[1]))
    
    for row in range(data.shape[1]):
        for col in range(row,data.shape[1]):
            if col == row:
                c[row, col] = 1.0
            else:
                cov = np.cov(data.iloc[:, row], data.iloc[:, col])[0][1]
                c[row, col] = cov / (sigma[row] * sigma[col])
                c[col, row] = c[row, col]
    return c

def get_common(results, top):
    '''
        Obtiene los resultados comunes de los primeros top resultados de cada pais
    '''
    common = set(u['href'] for u in results[0]['search_results']['results'][:top])
    for c in results[1:]:
        common.intersection_update(u['href'] for u in c['search_results']['results'][:top])
    return common

def build_aligned_ranks(rank_list, universe, top, absent_policy="max_plus_one"):
    """
        Genera una lista de la posicion de los elementos de universe en rank_list.
        Si un elemento de universe no está en rank_list, se le asigna una posición
        según absent_policy.
    rank_list: lista de dicts con clave 'href', en orden de ranking.
    universe: lista/iterable con todos los hrefs a alinear.
    top: entero, cuántos tomas de rank_list.
    absent_policy:
      - 'max_plus_one': rank = len(universe) + 1 a los ausentes
      - '1.5top': rank = int(1.5 * top) a los ausentes
    """
    pos = {item['href']: i+1 for i, item in enumerate(rank_list)}  # 1..len(list)
    if absent_policy == "max_plus_one":
        absent_rank = len(universe) + 1
    elif absent_policy == "1.5top":
        absent_rank = int(1.5 * top)
    else:
        absent_rank = len(universe) + 1
    return [pos.get(u, absent_rank) for u in universe]

def results_order_correlation(results, universe, top, method="spearman", absent_policy="max_plus_one", only_common=False, common_by_order=False):
    """
    Calcula la matriz de correlación entre los rankings de diferentes países.
    universe: lista/iterable con todos los hrefs a alinear.
    top: entero, cuántos resultados tomar de cada país.
    results: lista de dicts con llaves ["country"] y ["search_results"]["results"]
    method: "spearman" (rho) o "kendall" (tau-b)
    absent_policy:
      - 'max_plus_one': rank = len(universe) + 1 a los ausentes
      - '1.5top': rank = int(1.5 * top) a los ausentes
    only_common: si True, solo usa los resultados comunes a todos los países
    common_by_order: si True y only_common es True, alinea los rankings según el orden común,
                  si False (default), usa las posiciones de los elementos comunes en el ranking original
    Returns: (corr_df, pval_df, aligned)
      corr_df: DataFrame con matriz de correlaciones
      pval_df: DataFrame con matriz de p-valores
      aligned: dict país -> lista de posiciones alineadas
    """
    
    countries = [c["country"] for c in results]

    if only_common:
        common_universe = [u for u in universe if u in get_common(results, top)]
        aligned = {}
        for i in common_universe:
            print(i)
        if common_by_order:
            for c in results:
                common = [r for r in c["search_results"]["results"][:top] if r["href"] in common_universe]
                if (c["country"]=='Bolivia') or c["country"]=='Spain':
                    print("Common for {}:".format(c["country"]))
                    for r in common:
                        print("\t{}".format(r['href']))
                aligned[c["country"]] = build_aligned_ranks(
                        common, common_universe, top, absent_policy=absent_policy
                    )
        else:
            for c in results:
                top_list = c['search_results']['results'][:top]
                pos = {item['href']: i+1 for i, item in enumerate(top_list)}  # 1..len(list)
                aligned[c["country"]] = [pos[u] for u in common_universe]

    else:
        aligned = {
            c["country"]: build_aligned_ranks(
                c["search_results"]["results"][:top], universe, top, absent_policy=absent_policy
            )
            for c in results
        }

    corr_data, pval_data = [], []
    for c1 in countries:
        row_corr, row_p = [], []
        x = aligned[c1]
        for c2 in countries:
            y = aligned[c2]
            if method == "spearman":
                r, p = spearmanr(x, y)
            elif method == "kendall":
                r, p = kendalltau(x, y)
            else:
                raise ValueError("method debe ser 'spearman' o 'kendall'")
            row_corr.append(0.0 if (r is None or math.isnan(r)) else float(r))
            row_p.append(1.0 if (p is None or math.isnan(p)) else float(p))
        corr_data.append(row_corr)
        pval_data.append(row_p)

    corr_df = pd.DataFrame(corr_data, index=countries, columns=countries)
    p_df    = pd.DataFrame(pval_data, index=countries, columns=countries)
    return corr_df, p_df, aligned

def graph_result_coincidence_heatmap(resultados, tops, show=True, only_common=False):
    import matplotlib.pyplot as plt
    import seaborn as sns
    # Crear matriz de coincidencias de href
    coincidencias = {}
    for pais in resultados:
        coincidencias[pais["country"]] = {}
        for pais2 in resultados:
            coincidencias[pais["country"]][pais2["country"]] = 0

    if only_common:
        common = get_common(resultados, tops[-1])
        for i in range(len(tops)-1):
            for pais in resultados:
                for pais2 in resultados:
                    for result in pais["search_results"]["results"][tops[i]:tops[i+1]]:
                        for result2 in pais2["search_results"]["results"][tops[i]:tops[i+1]]:
                            if (result["href"] == result2["href"]) and (result["href"] in common):
                                coincidencias[pais["country"]][pais2["country"]] += 1
            # Crear matriz de coincidencias
            matriz = []
            for pais in coincidencias:
                matriz.append([coincidencias[pais][pais2] for pais2 in coincidencias])

            if show:
                # Crear heatmap
                plt.figure(figsize=(10, 10))
                sns.heatmap(matriz, annot=True, fmt="d", xticklabels=coincidencias.keys(), yticklabels=coincidencias.keys(), vmin=0, vmax=tops[i+1], cmap="YlGnBu_r")
                plt.title("Matriz de coincidencias de resultados de búsqueda de 'Corrupción' (Top {})".format(tops[i+1]))

                # Guardar heatmap
                # plt.savefig("Coincidencias/heatmap_violencia.png")

                plt.show()
        else:    
            for i in range(len(tops)-1):
                for pais in resultados:
                    for pais2 in resultados:
                        for result in pais["search_results"]["results"][tops[i]:tops[i+1]]:
                            for result2 in pais2["search_results"]["results"][tops[i]:tops[i+1]]:
                                if result["href"] == result2["href"]:
                                    coincidencias[pais["country"]][pais2["country"]] += 1

                # Crear matriz de coincidencias
                matriz = []
                for pais in coincidencias:
                    matriz.append([coincidencias[pais][pais2] for pais2 in coincidencias])
                
                if show:
                    # Crear heatmap
                    plt.figure(figsize=(10, 10))
                    sns.heatmap(matriz, annot=True, fmt="d", xticklabels=coincidencias.keys(), yticklabels=coincidencias.keys(), vmin=0, vmax=tops[i+1], cmap="YlGnBu_r")
                    plt.title("Matriz de coincidencias de resultados de búsqueda de 'Corrupción' (Top {})".format(tops[i+1]))

                    # Guardar heatmap
                    # plt.savefig("Coincidencias/heatmap_violencia.png")

                    plt.show()

        return pd.DataFrame(np.array(matriz), index=coincidencias.keys(), columns=coincidencias.keys())


# ------------------------------------------------
# ANALISIS DE RESULTADOS DE BUSQUEDA
# ------------------------------------------------

def parse_choices(x):
    '''
    Convierte una cadena de respuestas múltiples en una lista de incisos.
    '''
    if pd.isna(x):
        return []
    s = str(x).strip().lower()
    if not s:
        return []
    else:
        return sorted(set(s.split()))

def choices_to_ordinal(choices, ordinal_dict=None):
    '''
    Dada una lista de incisos (respuestas a una pregunta), las convierte a sus valores ordinales y devuelve su promedio.
    Asume que los incisos son letras (a, b, c, ...), y asigna sus valores ordinales segun ordinal_dict (dict inciso -> valor),
    si ordinal_dict es None, asigna a=1, b=2, c=3, etc.
    Si la lista está vacía, devuelve NaN.
    '''
    if not choices:
        return np.nan
    if ordinal_dict:
        values = [ordinal_dict.get(c) for c in choices if c.isalpha()]
    else:
        values = [ord(c) - ord('a') + 1 for c in choices if c.isalpha()]
    return np.mean(values) if values else np.nan

def analyze_pages(df, multi_choice_questions, possible_answers, ordinal=False):
    '''
    Analiza las respuestas a las preguntas para cada pagina, generando un resumen de las caracteristicas
    de cada pagina basado en las respuestas al cuestionario, produciendo una columna por cada inciso.
    multi_choice_questions: lista de preguntas que permiten respuestas múltiples (incisos) 
        Su valor se mide por porcentaje de eleccion; si no es multichoice, se asigna 1 al inciso más elegido y 0 a los demás.
    possible_answers: dict pregunta -> lista de incisos posibles (para asegurar consistencia en columnas
    '''
    
    df2 = df.copy()
    df2['incisos'] = df2['incisos'].apply(parse_choices)
    
    single_questions = sorted(set(df2['pregunta'].unique()) - set(multi_choice_questions))
    ordinal_questions = {'informacion_presentada': {'a': 1, 'b': 2, 'c': 3}, 
                         'consecuencias': {'d': 0, 'c': 1, 'b': 2, 'a': 3}, 
                         'intencion': {'f': 0, 'd': 1, 'a': 2, 'b': 3, 'c': 4, 'e': 5}, 
                         'localidad_problema': {'d': 0, 'a': 1, 'b': 2, 'c': 3}, 
                         'involucrados': [{'e': 1, 'f': 2, 'a': 3, 'b': 4, 'h': 5, 'd': 1, 'c': 2, 'g': 3},     # nivel de alcance
                                          {'e': 2, 'f': 2, 'a': 2, 'b': 2, 'h': 2, 'd': 1, 'c': 1, 'g': 1}]}    # sector (privado: 1, publico: 2)

    rows = []
    for q in single_questions:
    #for q in sorted(df2['pregunta'].unique()):
        allowed = possible_answers.get(q, [])
        sub = df2[df2['pregunta'].eq(q)].copy()
        n_by_page = sub.groupby('pagina').size().rename('n') # numero de respuestas a la pregunta por pagina
        exploded = sub[['pagina', 'incisos']].explode('incisos')
        exploded = exploded.dropna(subset=['incisos'])
        counts = exploded.groupby(['pagina', 'incisos']).size().rename('count')
        wide_counts = counts.unstack('incisos').reindex(columns=allowed, fill_value=0).fillna(0)
        winners = wide_counts.idxmax(axis=1)

        onehot = pd.DataFrame(0, index=wide_counts.index, columns=wide_counts.columns, dtype=int)

        # opcional: si una fila es todo 0, dejarla toda en 0 (sin ganador)
        has_signal = wide_counts.ne(0).any(axis=1)
        winners = winners.where(has_signal, None)

        valid = winners.notna()
        r = np.where(valid)[0]
        c = onehot.columns.get_indexer(winners[valid])
        onehot.values[r, c] = 1
        # --------------------------------------

        onehot.columns = [f"{q}__{c}" for c in onehot.columns]
        rows.append(onehot)
    single_summary = pd.concat(rows, axis=1) if rows else pd.DataFrame()
    mx = single_summary.max(axis=1)
    single_summary = single_summary.eq(mx, axis=0).astype(int)

    if not ordinal:
        rows = []
        for q in multi_choice_questions:
        #for q in sorted(df2['pregunta'].unique()):
            allowed = possible_answers.get(q, [])
            sub = df2[df2['pregunta'].eq(q)].copy()
            
            n_by_page = sub.groupby('pagina').size().rename('n') # numero de respuestas a la pregunta por pagina
        
            exploded = sub[['pagina', 'incisos']].explode('incisos')
            exploded = exploded.dropna(subset=['incisos'])
            counts = exploded.groupby(['pagina', 'incisos']).size().rename('count')
            wide_counts = counts.unstack('incisos').reindex(columns=allowed, fill_value=0).fillna(0)
            wide_pct = wide_counts.div(n_by_page, axis=0).fillna(0)
            wide_pct.columns = [f"{q}__{c}" for c in wide_pct.columns]
            rows.append(wide_pct)

        multi_summary = pd.concat(rows, axis=1) if rows else pd.DataFrame()
        return pd.concat([single_summary, multi_summary], axis=1)
    else:
        rows = []
        for q in ordinal_questions:
            sub = df2[df2['pregunta'].eq(q)].copy()
            if q != 'involucrados':
                sub[q] = sub['incisos'].apply(lambda inc: choices_to_ordinal(inc, ordinal_dict=ordinal_questions[q]))
                agg = sub.groupby('pagina')[q].mean().to_frame()
                rows.append(agg)
            else:
                sub[q+'_nivel'] = sub['incisos'].apply(lambda inc: choices_to_ordinal(inc, ordinal_dict=ordinal_questions[q][0]))
                sub[q+'_sector'] = sub['incisos'].apply(lambda inc: choices_to_ordinal(inc, ordinal_dict=ordinal_questions[q][1]))
                
                agg = sub.groupby('pagina')[[q+'_nivel', q+'_sector']].mean()
                rows.append(agg)
        
        ordinal_summary = pd.concat(rows, axis=1) if rows else pd.DataFrame()

        rows = []
        non_ordinal_multi_choice_questions = [q for q in multi_choice_questions if q not in ordinal_questions]
        for q in non_ordinal_multi_choice_questions:
        #for q in sorted(df2['pregunta'].unique()):
            allowed = possible_answers.get(q, [])
            sub = df2[df2['pregunta'].eq(q)].copy()
            
            n_by_page = sub.groupby('pagina').size().rename('n') # numero de respuestas a la pregunta por pagina
        
            exploded = sub[['pagina', 'incisos']].explode('incisos')
            exploded = exploded.dropna(subset=['incisos'])
            counts = exploded.groupby(['pagina', 'incisos']).size().rename('count')
            wide_counts = counts.unstack('incisos').reindex(columns=allowed, fill_value=0).fillna(0)
            wide_pct = wide_counts.div(n_by_page, axis=0).fillna(0)
            wide_pct.columns = [f"{q}__{c}" for c in wide_pct.columns]
            rows.append(wide_pct)

        multi_summary = pd.concat(rows, axis=1) if rows else pd.DataFrame()
        return pd.concat([single_summary, ordinal_summary, multi_summary], axis=1)

def merge_features(df_rankings, df_resumen):
    '''
    Convierte el DataFrame de rankings (ancho) a formato largo y lo mergea con el resumen de caracteristicas por pagina.'''
    rank_long = (
        df_rankings
        .rename_axis('country')
        .stack(dropna=False)
        .reset_index()
        .rename(columns={'level_1': 'rank', 0: 'page_id'})
    )
    rank_long.dropna(subset=['page_id'], inplace=True)
    rank_long['rank'] += 1

    rank_feat = rank_long.merge(
        df_resumen.reset_index().rename(columns={'pagina': 'page_id'}),
        on='page_id',
        how='left'
    )
    return rank_feat

def compute_rank_feature_correlation(df_rankings, df_resumen, segment_size=10, segment_start=1):
    '''
    Para cada país, segmenta sus páginas según su posición en el ranking (segmentos de tamaño segment_size), 
    calcula el promedio de las características de las páginas en cada segmento, 
    y luego calcula la matriz de correlación entre los segmentos (usando las características promedio) a nivel país.
    '''
    rank_feat = merge_features(df_rankings, df_resumen)
    rank_feat = rank_feat[rank_feat['rank'].between(segment_start, 100)]
    rank_feat['segmento'] = ((rank_feat['rank'] - segment_start) // segment_size + 1).astype(int)
    seg = (
        rank_feat
        .groupby(['country', 'segmento'], as_index=False)[df_resumen.columns.tolist()]
        .mean()
    )
    
    wide = seg.pivot(index='country', columns='segmento', values=df_resumen.columns.tolist())
    wide.columns = [f'{m}_{s}' for m, s in wide.columns]
    return wide.T.corr()

def compute_feature_distance(feats, metric="euclidean"):
    from sklearn.preprocessing import StandardScaler
    from scipy.spatial.distance import pdist, squareform

    X = StandardScaler().fit_transform(feats.values)
    dists = squareform(pdist(X, metric=metric))
    return pd.DataFrame(dists, index=feats.index, columns=feats.index)

def compute_rank_feature_distance(
    df_rankings,
    df_feat,
    segment_size=10,
    segment_start=1,
    segment_end=100,
    onehot_cols=None,          # <- lista de columnas one-hot en df_feat
    metric="euclidean",
    expanded=False,           # si True, concatena mean, std, cum, diff, delta_cum; si False (default), solo usa mean
    by='All',                 # caracteristicas a usar para calcular distancias
    threshold=0.6,          # umbral de relevancia para seleccionar características (si by != 'All')
    k=3,
    compare_to=None,          # características a comparar
):
    """
    Calcula representaciones agregadas de características por segmentos del ranking
    y, opcionalmente, selecciona las características más relevantes con respecto
    a una o varias variables objetivo.

    La función convierte rankings en formato ancho a formato largo, los combina con
    las características de cada página y agrega dichas características por país y
    por segmento del ranking. Después puede:

    - devolver todas las características agregadas,
    - calcular una matriz de distancias entre países,
    - o filtrar características relevantes según su relación con `compare_to`.

    Parameters
    ----------
    df_rankings : pandas.DataFrame
        DataFrame de rankings en formato ancho. Cada columna representa una posición
        en el ranking y cada fila corresponde a un país.
    df_feat : pandas.DataFrame
        DataFrame de características por página. Debe poder vincularse con los IDs
        de página presentes en `df_rankings`.
    segment_size : int, default=10
        Tamaño de cada segmento del ranking.
    segment_start : int, default=1
        Posición inicial del ranking a considerar.
    segment_end : int, default=100
        Posición final del ranking a considerar.
    onehot_cols : list[str] or None, default=None
        Lista de columnas one-hot que deben agregarse de manera especial.
    metric : str, default="euclidean"
        Métrica de distancia usada por `compute_feature_distance`.
    expanded : bool, default=False
        Si es True, devuelve también la representación expandida de características.
    by : {"All", "correlation", "question", "top_k_options"}, default="All"
        Estrategia de selección de características:
        - "All": usa todas las características agregadas.
        - "correlation": filtra por umbral de correlación.
        - "question": selecciona todas las features de las preguntas relevantes.
        - "top_k_options": selecciona los k incisos más relevantes por pregunta.
    threshold : float or None, default=0.6
        Umbral mínimo de relevancia para filtrar preguntas o características.
    k : int, default=3
        Número de incisos más relevantes por pregunta cuando `by="top_k_options"`.
    compare_to : pandas.Series or pandas.DataFrame or None, default=None
        Variable o variables objetivo contra las cuales evaluar relevancia.

    Returns
    -------
    tuple
        Dependiendo de la configuración, devuelve una tupla con:
        - matriz de distancias o relevancias,
        - y el DataFrame ancho de características seleccionadas.

    Notes
    -----
    Cuando `compare_to` no es None y `by != "All"`, la selección de características
    se realiza a partir de su relevancia respecto a la(s) variable(s) objetivo.
    """

    def to_wide(seg_df, tag):
        wide = seg_df.pivot(index="country", columns="segmento", values=feature_cols)
        wide.columns = [f"{c}_{tag}_s{s}" for c, s in wide.columns]
        return wide
    
    if onehot_cols is None:
        onehot_cols = []

    # 1) rankings ancho -> largo y merge features
    feat = df_feat.reset_index().rename(columns={"pagina": "page_id"})
    rank_feat = merge_features(df_rankings, df_feat)

    # 3) segmentación
    rank_feat = rank_feat[rank_feat["rank"].between(segment_start, segment_end)].copy()
    rank_feat["segmento"] = ((rank_feat["rank"] - segment_start) // segment_size + 1).astype(int)

    # 4) columnas a agregar (solo numéricas, excluyendo id)
    feature_cols = [c for c in df_feat.columns if c != "pagina"]
    # Asegura que onehot_cols estén en feature_cols
    onehot_cols = [c for c in onehot_cols if c in feature_cols]
    cont_cols = [c for c in feature_cols if c not in onehot_cols]

    if onehot_cols:
        rank_feat[onehot_cols] = rank_feat[onehot_cols].fillna(0).astype(float)
    
    # 5) one-hot: sum (cuentas); continuas: mean
    # (importante: one-hot debería ser 0/1; si hay NaN, lo tratamos como 0)
    rank_feat[onehot_cols] = rank_feat[onehot_cols].fillna(0).astype(float)

    seg_n = (
        rank_feat.groupby(["country","segmento"], as_index=False)
                .size()
                .rename(columns={"size": "n_seg"})
    )
    agg_mean = {c: "mean" for c in feature_cols}
    agg_std = {c: 'std' for c in feature_cols}

    seg_mean = (
        rank_feat
        .groupby(["country", "segmento"], as_index=False)
        .agg(agg_mean)
        .merge(seg_n, on=["country", "segmento"], how="left")
    )

    seg_mean = (
        rank_feat
        .groupby(["country", "segmento"], as_index=False)
        .agg(agg_mean)
        .merge(seg_n, on=["country", "segmento"], how="left")
    )

    if expanded:
        seg_std = (
            rank_feat
            .groupby(["country", "segmento"], as_index=False)
            .agg(agg_std)
            .merge(seg_n, on=["country", "segmento"], how="left")
        )
        seg_std[feature_cols] = seg_std[feature_cols].fillna(0) # para evitar NaN en segmentos con 1 solo elemento
        
        # Ordenar para acumulados/diffs
        seg_mean = seg_mean.sort_values(["country", "segmento"]).reset_index(drop=True)
        seg_std = seg_std.sort_values(["country", "segmento"]).reset_index(drop=True)

        # ====== acumulado: cumulative mean ponderado por n_seg ======
        # cum_sum_f = cumsum(mean_f * n_seg)
        # cum_n     = cumsum(n_seg)
        # cum_f     = cum_sum_f / cum_n
        tmp = seg_mean[["country", "segmento", "n_seg"] + feature_cols].copy()
        for c in feature_cols:
            tmp[c] = tmp[c] * tmp["n_seg"]

        cum_sum = tmp.groupby("country", as_index=False)[feature_cols].cumsum()
        cum_n   = tmp.groupby("country", as_index=False)["n_seg"].cumsum()

        seg_cum = seg_mean[["country", "segmento"]].copy()
        seg_cum["n_seg"] = seg_mean["n_seg"].values
        for c in feature_cols:
            seg_cum[c] = (cum_sum[c] / cum_n).fillna(0)

        # ====== diff entre segmentos consecutivos (sobre mean) ======
        seg_diff = seg_mean[["country", "segmento"]].copy()
        seg_diff["n_seg"] = seg_mean["n_seg"].values
        seg_diff[feature_cols] = (
            seg_mean.groupby("country")[feature_cols].diff().fillna(0)
        )

        # ====== diferencia entre segmento actual y acumulado ======
        seg_delta_cum = seg_mean[["country", "segmento"]].copy()
        seg_delta_cum["n_seg"] = seg_mean["n_seg"].values
        seg_delta_cum[feature_cols] = (seg_mean[feature_cols] - seg_cum[feature_cols]).fillna(0)

        # 6) pivot a wide
    
        wide_mean = to_wide(seg_mean, "mean")
        wide_std = to_wide(seg_std, "std")
        wide_cum = to_wide(seg_cum, "cum")
        wide_diff = to_wide(seg_diff, "diff")
        wide_delta_cum = to_wide(seg_delta_cum, "delta_cum")

        wides = {
            "mean": wide_mean,
            "std": wide_std,
            "cum": wide_cum,
            "diff": wide_diff,
            "delta_cum": wide_delta_cum,
        }

        wide = pd.concat(wides.values(), axis=1)

    else:
        wide = to_wide(seg_mean, "mean")

    if by == 'All':
        # 7) distancias
        return compute_feature_distance(wide, metric=metric), wide
    elif by != 'All' and compare_to is not None:
        common_idx = wide.index.intersection(compare_to.index)
        wide2 = wide.loc[common_idx]
        compare_to2 = compare_to.loc[common_idx]
        
        relevance_df = feature_relevance_multitarget(wide2, compare_to2)
        relevant_feats = get_most_relevant(relevance_df, threshold=threshold, by=by, k=k)
        return compute_feature_distance(wide2[relevant_feats], metric=metric), wide2[relevant_feats]


def inverse_rank_weight(rank, start_at_1=1):
    return 1.0 / (rank + start_at_1)

def compute_rank_feature_ponderate(
    df_rankings,
    df_feat,
    compute_start=1,
    compute_end=100,
    metric="euclidean",
    weight_func=None,  # función de peso opcional, por defecto w(rank) = 1/log2(rank+1)
):
    '''
    Para cada país, calcula un perfil de características ponderado por la posición en el ranking (usando weight_func para asignar pesos a cada posición),
    y luego calcula la matriz de distancias entre los perfiles de características a nivel país.
    '''
    from sklearn.preprocessing import StandardScaler
    from scipy.spatial.distance import pdist, squareform

    def inverse_log_weight(rank):
        return 1.0/np.log2(rank + 1)

    weight = None
    
    if weight_func is None:
        weight = inverse_log_weight
    else:
        weight = weight_func

    # 1) rankings ancho -> largo y merge features
    rank_feat = merge_features(df_rankings, df_feat)

    # 3) pesos
    rank_feat = rank_feat[rank_feat["rank"].between(compute_start, compute_end)]
    rank_feat['rank'] -= compute_start - 1
    d = rank_feat[rank_feat['rank'] <= compute_end - compute_start + 1].copy()
    d['w'] = d['rank'].map(weight)
    
    # 4) suma ponderada
    feature_cols = [c for c in d.columns if c not in ['country', 'rank', 'page_id', 'w']]
    num = d.assign(**{col: d[col] * d['w'] for col in feature_cols}) \
            .groupby("country")[feature_cols].sum()
    
    den = d.groupby("country")['w'].sum()
    profile = num.div(den, axis=0)
    profile = profile.apply(lambda col: col.fillna(col.mean()), axis=0)
    
    X = profile.values
    X = StandardScaler().fit_transform(X)
    
    # 7) distancias
    dists = squareform(pdist(X, metric=metric))
    return pd.DataFrame(dists, index=profile.index, columns=profile.index), profile


# ------------------------------------------------
# FUNCIONES AUXILIARES
# ------------------------------------------------

# Función Haversine (km)
def haversine_km(coord1, coord2):
    R = 6371.0088  # radio medio de la Tierra en km
    lat1, lon1 = map(math.radians, coord1)
    lat2, lon2 = map(math.radians, coord2)
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = (math.sin(dlat/2)**2
         + math.cos(lat1) * math.cos(lat2) * math.sin(dlon/2)**2)
    c = 2 * math.asin(math.sqrt(a))
    return R * c

def upper_tri_vals(df):
    mask = np.triu(np.ones(df.shape), k=1).astype(bool)
    return df.where(mask).stack()

import matplotlib.pyplot as plt
from cycler import cycler

import matplotlib.pyplot as plt

# Paleta Okabe–Ito (colorblind-friendly)
_OKABE_ITO = [
    "#000000", "#E69F00", "#56B4E9", "#009E73",
    "#F0E442", "#0072B2", "#D55E00", "#CC79A7"
]
_MARKERS = ["o", "s", "^", "D", "v", "P", "X", "*", "<", ">", "h", "p", "+", "x"]
_LINESTYLES = ["-", "--", "-.", ":"]

def build_style_map(labels):
    """
    labels: iterable de nombres (comparaciones). Devuelve dict label -> kwargs de plot.
    Consistente para cualquier cantidad: cicla sobre listas.
    """
    labels = list(labels)
    style_map = {}
    for i, lab in enumerate(labels):
        style_map[lab] = dict(
            color=_OKABE_ITO[i % len(_OKABE_ITO)],
            marker=_MARKERS[i % len(_MARKERS)],
            linestyle=_LINESTYLES[i % len(_LINESTYLES)],
        )
    return style_map

def plot_wide(df_wide, title, ylab, style_map, ax=None, legend_ncols=2, log_scale=False, ylim=None, legend_out=True, show_legend=True):
    """
    df_wide: index = top, columns = comparacion, values = y
    style_map: dict comparacion -> estilo (color/marker/linestyle)
    """
    if ax is None:
        fig, ax = plt.subplots()

    x = df_wide.index.to_numpy()

    for col in df_wide.columns:
        y = df_wide[col].to_numpy()
        kw = style_map.get(col, {})
        ax.plot(x, y, label=str(col), **kw)

    ax.set_title(title)
    ax.set_xlabel("top")
    ax.set_ylabel(ylab)
    ax.grid(True, alpha=0.3)
    if show_legend:
        if legend_out:
            leg = ax.legend(loc="center left", bbox_to_anchor=(1.02, 0.5), frameon=False)
        else:
            leg = ax.legend(ncols=legend_ncols, frameon=False)
        leg.set_title("Comparación")
    if log_scale:
        ax.set_yscale("log")
    if ylim is not None:
        ax.set_ylim(ylim)
    plt.tight_layout()
    return ax

def mantel_spearman(A, B, n_perm=10000, seed=0):
    rng = np.random.default_rng(seed)
    A = np.asarray(A)
    B = np.asarray(B)
    iu = np.triu_indices_from(A, k=1)
    x = A[iu]
    y = B[iu]
    m = np.isfinite(x) & np.isfinite(y)
    x, y = x[m], y[m]

    obs = spearmanr(x, y).statistic

    # Permuta etiquetas (mismo shuffle en filas/cols)
    count = 0
    n = A.shape[0]
    for _ in range(n_perm):
        perm = rng.permutation(n)
        Ap = A[perm][:, perm]
        xp = Ap[iu]
        mp = np.isfinite(xp) & np.isfinite(y)
        rp = spearmanr(xp[mp], y[mp]).statistic
        if abs(rp) >= abs(obs):
            count += 1

    p = (count + 1) / (n_perm + 1)
    return obs, p


def feature_relevance(feats, target):
    """
    Calcula la relevancia de diferentes caracteristicas (feats) con respecto a una variable objetivo (target).
    Devuelve correlacion d Spearman y p-value para cada caracteristica.
    """
    y = target.reindex(feats.index)
    rows = []
    for feat in feats.columns:
        x = feats[feat]
        m = x.notna() & y.notna()
        corr, p = spearmanr(y[m], x[m])
        rows.append((feat, corr, p, m.sum()))

    res = pd.DataFrame(rows, columns=['feature', 'rh', 'p', 'n'])
    # correccion por multiples pruebas (FDR Benjamini-Hochberg)
    res['p_fdr'] = multipletests(res['p'], method='fdr_bh')[1]
    res = res.sort_values(['p_fdr', 'rh'], ascending=[True, False])
    return res

def feature_relevance_multitarget(feats, compare_to_targets):
    """
    Calcula la relevancia de diferentes caracteristicas (feats) con respecto a varias variables objetivo (compare_to_targets).
    Devuelve un DataFrame con correlacion d Spearman y p-value para cada caracteristica y cada variable objetivo.
    """
    rows = []
    for target in compare_to_targets.columns:
        res = feature_relevance(feats, compare_to_targets[target])
        res['target'] = target
        rows.append(res)
    return pd.concat(rows, ignore_index=True)

def get_most_relevant(relevance_df, threshold=0.6, by='correlation', k=3):
    """
    Dado un DataFrame de relevancia (con columnas 'feature', 'rh', 'p_fdr', 'target'), devuelve las características más relevantes para cada variable objetivo.
    Se puede filtrar por un umbral mínimo de correlación (threshold).
    by: 'correlation' (default) ordena por valor absoluto de correlación.
        'question' devuelve todos los incisos de las preguntas mas relevantes, ordenados por pregunta y luego por correlación.
        'top_k_options' devuelve los k incisos más relevantes de las preguntas mas relevantes (asumiendo que las features tienen formato 'pregunta__inciso').
    """
    relevant = relevance_df.copy()
    if threshold is None:
        threshold = 0.6

    if by == 'correlation':
        return relevant[relevance_df['rh'].abs() >= threshold]['feature'].dropna().tolist()
    elif by == 'question':
        relevant['question'] = relevant['feature'].apply(lambda f: f.split('__')[0] if '__' in f else f)
        questions = relevant[relevance_df['rh'].abs() >= threshold]['question'].unique()
        return relevant.where(relevant['question'].isin(questions))['feature'].dropna().tolist()
    elif by == 'top_k_options':
        relevant['question'] = relevant['feature'].apply(lambda f: f.split('__')[0] if '__' in f else f) 
        relevant['option'] = relevant["feature"].str.replace( r"_(?:mean|std|cum|diff|delta_cum)_s\d+$", "", regex=True ) 
        relevant['abs_rh'] = relevant['rh'].abs()  
        option_scores = ( 
            relevant
            .groupby(['question', 'option'], as_index=False) 
            .agg(score=('abs_rh', 'max')) 
        ) 
        relevant_questions = ( 
            option_scores 
            .groupby('question', as_index=False) 
            .agg(question_score=('score', 'max'))
        )
        relevant_questions = relevant_questions.loc[relevant_questions['question_score'] >= threshold, 'question']

        top_options = (
            option_scores[option_scores['question'].isin(relevant_questions)]
            .sort_values(['question', 'score'], ascending=[True, False])
            .groupby('question', group_keys=False)
            .head(k)
        )

        # pares (question, option) seleccionados
        selected = top_options[['question', 'option']].drop_duplicates()

        return (
            relevant.merge(selected, on=['question', 'option'], how='inner')['feature']
            .drop_duplicates()
            .dropna()
            .tolist()
        )
    else:
        raise ValueError("by debe ser 'correlation', 'question' o 'top_k_options'")