import numpy as np
import pandas as pd
import math
from scipy.stats import kendalltau, spearmanr, pearsonr
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

def results_correlation(results, universe, top, method="spearman", absent_policy="max_plus_one", only_common=False, common_by_order=False):
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

def graph_coincidence_heatmap(resultados, tops, show=True, only_common=False):
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
    Convierte una cadena de respuestas múltiples en una lista de respuestas.
    '''
    if pd.isna(x):
        return []
    s = str(x).strip().lower()
    if not s:
        return []
    else:
        return sorted(set(s.split()))
    
def analyze_single_choice(series):
    '''
    Analiza respuestas de opción única.
    '''
    letter2num = {chr(ord('a') + i): i + 1 for i in range(9)}
    vals = series.apply(lambda lst: lst[0] if lst else np.nan).dropna()
    if vals.empty:
        return np.nan
    m = vals.mode(dropna=True)
    winner = m.iloc[0]
    return letter2num.get(str(winner).lower(), np.nan)

def analyze_pages(df, multi_choice_questions, possible_answers):
    '''
    Analiza las respuestas a las preguntas para cada pagina.
    '''
    df2 = df.copy()
    df2['incisos'] = df2['incisos'].apply(parse_choices)

    single_questions = sorted(set(df2['pregunta'].unique()) - set(multi_choice_questions))
    single_summary = (
        df2[df2['pregunta'].isin(single_questions)]
        .groupby(['pagina', 'pregunta'])['incisos']
        .apply(analyze_single_choice)
        .unstack('pregunta')
    )

    def normalize_single_summary(df_single, possible_answers):
        out = df_single.copy()
        for col in out.columns:
            K = len(possible_answers.get(col, []))
            out[col] = out[col] / K
        return out
    
    single_summary = normalize_single_summary(single_summary, possible_answers)
    
    rows = []
    for q in multi_choice_questions:
        allowed = possible_answers.get(q, [])
        sub = df2[df2['pregunta'].eq(q)].copy()

        n_by_page = sub.groupby('pagina').size().rename('n') # numero de respuestas a la pregunta por pagina

        exploded = sub[['pagina', 'incisos']].explode('incisos')
        exploded = exploded.dropna(subset=['incisos'])
        counts = exploded.groupby(['pagina', 'incisos']).size().rename('count')
        wide_counts = counts.unstack('incisos').reindex(columns=allowed, fill_value=0)
        wide_pct = wide_counts.div(n_by_page, axis=0).fillna(0)
        wide_pct.columns = [f"{q}__{c}" for c in wide_pct.columns]
        rows.append(wide_pct)

    multi_summary = pd.concat(rows, axis=1) if rows else pd.DataFrame()
    return pd.concat([single_summary, multi_summary], axis=1)

def compute_rank_feature_correlation(df_rankings, df_resumen, segment_size=10, segment_start=1):
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
    rank_feat = rank_feat[rank_feat['rank'].between(segment_start, 100)]
    rank_feat['segmento'] = ((rank_feat['rank'] - segment_start) // segment_size + 1).astype(int)
    print(rank_feat.head())
    seg = (
        rank_feat
        .groupby(['country', 'segmento'], as_index=False)[df_resumen.columns.tolist()]
        .mean()
    )
    
    wide = seg.pivot(index='country', columns='segmento', values=df_resumen.columns.tolist())
    wide.columns = [f'{m}_{s}' for m, s in wide.columns]
    return wide.T.corr()

def compute_rank_feature_distance(
    df_rankings,
    df_feat,
    segment_size=10,
    segment_start=1,
    onehot_cols=None,          # <- lista de columnas one-hot en df_feat
    metric="euclidean",
):
    from sklearn.preprocessing import StandardScaler
    from scipy.spatial.distance import pdist, squareform
    
    if onehot_cols is None:
        onehot_cols = []

    # 1) rankings ancho -> largo
    rank_long = (
        df_rankings.rename_axis("country")
        .stack(dropna=False)
        .reset_index()
        .rename(columns={"level_1": "rank", 0: "page_id"})
    )
    rank_long = rank_long.dropna(subset=["page_id"])
    rank_long["rank"] += 1

    # 2) merge features
    feat = df_feat.reset_index().rename(columns={"pagina": "page_id"})
    rank_feat = rank_long.merge(feat, on="page_id", how="left")

    # 3) segmentación
    rank_feat = rank_feat[rank_feat["rank"].between(segment_start, 100)]
    rank_feat["segmento"] = ((rank_feat["rank"] - segment_start) // segment_size + 1).astype(int)

    # 4) columnas a agregar (solo numéricas, excluyendo id)
    feature_cols = [c for c in df_feat.columns if c != "pagina"]
    # Asegura que onehot_cols estén en feature_cols
    onehot_cols = [c for c in onehot_cols if c in feature_cols]
    cont_cols = [c for c in feature_cols if c not in onehot_cols]
    
    # 5) one-hot: sum (cuentas); continuas: mean
    # (importante: one-hot debería ser 0/1; si hay NaN, lo tratamos como 0)
    rank_feat[onehot_cols] = rank_feat[onehot_cols].fillna(0).astype(float)

    seg_n = (
        rank_feat.groupby(["country","segmento"], as_index=False)
                .size()
                .rename(columns={"size": "n_seg"})
    )
    agg = {c: "mean" for c in cont_cols} | {c: "sum" for c in onehot_cols}

    seg = (
        rank_feat
        .groupby(["country", "segmento"], as_index=False)
        .agg(agg)
    )
    seg = seg.merge(seg_n, on=["country", "segmento"], how="left")
    # one-hot sum -> proporción en [0,1]
    seg[onehot_cols] = seg[onehot_cols].div(seg["n_seg"], axis=0).fillna(0)
    seg = seg.drop(columns=["n_seg"])
    
    # 6) pivot a wide
    wide = seg.pivot(index="country", columns="segmento", values=feature_cols)
    wide.columns = [f"{m}_{s}" for m, s in wide.columns]
    X = StandardScaler().fit_transform(wide.fillna(0))
    # 7) distancias
    dists = squareform(pdist(X, metric=metric))
    return pd.DataFrame(dists, index=wide.index, columns=wide.index), wide


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

