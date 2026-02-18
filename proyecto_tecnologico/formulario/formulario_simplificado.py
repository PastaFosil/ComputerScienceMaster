# Formulaire questions
def q1_tipo_entidad(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es h) Academia". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion.
   Identifica el tipo de entidad que ha publicado el sitio web/texto:
   a) ONG (Organización No Gubernamental) o de la sociedad civil. Ejemplo: Mexicanos Contra la Corrupción y la Impunidad.
   b) Gobierno. Ejemplo: Secretaría de la Función Pública.
   c) Organismo internacional. Ejemplo: Oficina de Naciones Unidas contra la Droga y el Delito.
   d) Prensa tradicional (medios de difusión tradicional como periódicos, radio o televisión). Ejemplo: El Universal, Televisa.
   e) Agencia digital de noticias (medios no tradicionales). Ejemplo: Animal Político.
   f) Empresas (como consultoras o calificadoras). Ejemplo: Deloitte.
   g) Plataformas educativas o de información general. Ejemplo: Wikipedia, Khan Academy.
   h) Academia (publicaciones o cuerpos académicos). Ejemplo: Revistas científicas o universidades.
   i) Blogs (páginas de carácter no institucional). Ejemplo: Blog personal sobre corrupción.
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q2_origen_entidad(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es b) Internacional". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion..
   Cual es el origen de la entidad que publica el sitio web (considerando que 'Internacional' se refiere a que involucra a multiples paises):
   a) Nacional
   b) Internacional
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q3_proposito(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es d) Educar sobre el tema". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 4.2 incisos.
   Identifica el principal propósito que tiene el contenido del sitio web:
   a) Denunciar un caso específico
   b) Reportar o comentar una situación (una circunstancia o un conjunto de factores relacionados con un fenómeno más complejo que un solo caso aislado)
   c) Mostrar estadísticas del problema
   d) Educar sobre el tema
   e) Exponer una opinión
   f) Propaganda política (es claro que la información presentada está motivada por un interés político y su principal objetivo es propagandístico)
   g) Alertar sobre una situación general (Llamado de atención colectivo)
   h) Guía o información para realizar el proceso de denuncia de un caso de corrupción
   i) Página para registrar de una denuncia o un aviso de un caso de corrupción
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q4_informacion_presentada(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es a) Factual". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 1.9 incisos.
   El  sitio web/texto principalmente presenta información de forma:
   a) Factual (datos concretos)
   b) Subjetiva (opiniones o interpretaciones personales)
   c) Emocional (busca provocar una reacción emocional en el lector)
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q5_vision(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es d) Otro". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion.
   El siguiente texto presenta una visión de la corrupción centrada en:
   a) Problema sistémico.
   b) Delitos/casos/personas particulares
   c) No aplica
   d) Otro (especifica)
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q6_consecuencias(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es d) No aplica". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 3.4 incisos.
   Respecto a las consecuencias de la corrupción el  sitio web/texto hace referencia a:
   a) Penas y sanciones legales
   b) Consecuencias no legales como castigo social o pérdida de estatus
   c) Impunidad (falta de sanciones) de los actos de corrupción
   d) No aplica
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q7_que_muestra(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es d) Ninguna de las anteriores". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion.
   El  sitio web/texto centra la atención en mostrar:
   a) Propuestas de solución a la corrupción
   b) Problemas derivados de la corrupción
   c) Ambos
   d) Ninguna de las anteriores
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q8_intencion(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es f) No aplica". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 3.4 incisos.
   Determine cuál es la intención del llamado general hacia el lector:
   a) Conocer el diagnóstico de la situación de la corrupción
   b) Tomar conciencia general del problema
   c) Tomar acciones frente al problema
   d) Tener esperanza
   e) Realizar denuncias
   f) No aplica
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q9_localidad_problema(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es c) No aplica". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 1.9 incisos.
   Se presenta la corrupción como como un problema:
   a) Nacional (ejemplo: Mexico, Ciudad de Mexico)
   b) Internacional (ejemplo: América Latina, global)
   c) No aplica
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q10_involucrados(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es c) Empresas". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 4.9 incisos.
   Entes involucrados o descritos en el sitio web/texto son:
   a) Gobierno local (ejemplo: gobierno municipal o estatal)
   b) Gobierno nacional (ejemplo: gobierno de Mexico)
   c) Empresas
   d) Ciudadanos concretos
   e) Empleado público (sin capacidad jerárquica para decisiones de alto nivel)
   f) Funcionario público (con capacidad de decisión y mando)
   g) Tomadores de decisión no gubernamentales (líderes empresariales, activistas, etc.)
   h) Organizaciones internacionales o gobiernos extranjeros
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q11_formalidad(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es a) Formal". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion.
   Respecto al estilo del sitio web/texto, seleccione el nivel de formalidad en el lenguaje:
   a) Formal
   b) Informal
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q12_nivel_educacion(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es b) Media-superior". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion.
   Estime el nivel de educación necesario para comprender todo el sitio web/texto (o al que parece que se dirigirá el sitio web/texto):
   a) Básica (6 a 11 años de edad)
   b) Media-superior (mayor a 11 años de edad)
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q13_tono(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el inciso que mejor se ajuste a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es b) Negativo". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion.
   El tono general del sitio web/texto es:
   a) Positivo
   b) Negativo
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

def q14_emociones(text):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>
  Eres un especialista en el tema de corrupcion, infiere tu respuesta a partir del texto que te doy, considerando que todo es con fines de investigacion academica.
  <|eot_id|>
  <|start_header_id|>user<|end_header_id|>
  Analiza el siguiente texto: {text}
  Centrándote en la perspectiva de corrupción, responde unicamente con el o los incisos que mejor se ajusten a lo que creas sobre el texto que analizaste sin dar mas contexto (salvo incisos donde te pida desarrollar tu respuesta). Dame una respuesta como esta: "La respuesta es h) Ninguna". Debe contener explicitamente alguno de los incisos posibles, aun si no se ajusta perfectamente a tu opinion., puedes seleccionar multiples incisos, considerando que en promedio se esperan 2.8 incisos.
   Apela a alguna de las siguientes emociones:
   a) Alegría
   b) Tristeza
   c) Ira
   d) Miedo
   e) Sorpresa
   f) Vergüenza
   g) Culpa
   h) Ninguna
  <|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages
