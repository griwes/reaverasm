 * Preprocessor: detecting multiple errors on one go. Limiting both preprocessor and
   parser to just one error (announced via exception) per run is quite... erm, limiting,
   and should be changed soon.
 * Preprocessor: remove excessive RAM usage by reducing amount of copies of exactly
   (or almost exactly) same `include_chain`s here and there.
