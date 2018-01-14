#ifndef CPP_JIEBA_C_ABI_H_H
#define CPP_JIEBA_C_ABI_H_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

typedef void* jieba_t;
jieba_t jieba_new(const char* dict_path, const char* hmm_path, const char* user_dict, const char* idf_path, const char* stop_word_path);
void jieba_free(jieba_t);

typedef struct {
  char** words;
  size_t len;
} jieba_words_t;

void jieba_words_free(jieba_words_t* words);

jieba_words_t* jieba_cut(jieba_t handle, const char* sentence, int is_hmm_used);
jieba_words_t* jieba_cut_all(jieba_t handle, const char* sentence);
jieba_words_t* jieba_cut_for_search(jieba_t handle, const char* sentence, int is_hmm_used);
jieba_words_t* jieba_tag(jieba_t handle, const char* sentence);
const char* jieba_lookup_tag(jieba_t handle, const char* str);
void jieba_add_user_word(jieba_t handle, const char* word);
void jieba_reset_separators(jieba_t handle, const char* sep);

typedef struct {
  size_t offset;
  size_t len;
} jieba_token_t;

typedef enum {
  JIEBA_TOKENIZE_MODE_DEFAULT = 0,
  JIEBA_TOKENIZE_MODE_SEARCH = 1,
} jieba_tokenize_mode_t;

jieba_token_t* jieba_tokenize(jieba_t handle, const char* sentence, jieba_tokenize_mode_t mode, int is_hmm_used);
void jieba_token_free(jieba_token_t* tokens);

typedef struct {
  char* word;
  double weight;
} jieba_word_weight_t;

jieba_words_t* jieba_extract(jieba_t handle, const char* sentence, int top_k);
jieba_word_weight_t* jieba_extract_with_weight(jieba_t handle, const char* sentence, int top_k);
void jieba_word_weight_free(jieba_word_weight_t* wws);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif // CPP_JIEBA_C_ABI_H_H
