#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"

extern "C" {
  #include "jieba.h"
}

using namespace std;

static jieba_words_t* convert_words(const std::vector<string>& words) {
    size_t len = words.size();
    jieba_words_t* res = static_cast<jieba_words_t*>(malloc(sizeof(jieba_words_t) * len));
    res->length = len;
    res->words = static_cast<char**>(malloc(sizeof(char*) * len));
    for (size_t i = 0; i < len; i++) {
        res->words[i] = strdup(words[i].c_str());
    }
    return res;
}

static jieba_token_t* convert_tokens(const std::vector<cppjieba::Word>& words) {
    size_t len = words.size();
    jieba_token_t* res = static_cast<jieba_token_t*>(malloc(sizeof(jieba_token_t) * (len + 1)));
    for (size_t i = 0; i < len; i++) {
      auto word = words[i];
      res[i].offset = word.offset;
      res[i].length = word.word.size();
      res[i].unicode_offset = word.unicode_offset;
      res[i].unicode_length = word.unicode_length;
    }
    res[words.size()].offset = 0;
    res[words.size()].length = 0;
    res[words.size()].unicode_offset = 0;
    res[words.size()].unicode_length = 0;
    return res;
}

static jieba_word_weight_t* convert_word_weights(const std::vector<std::pair<std::string, double> >& words) {
  jieba_word_weight_t* res = static_cast<jieba_word_weight_t*>(malloc(sizeof(jieba_word_weight_t) * (words.size() + 1)));
  for (size_t i = 0; i < words.size(); i++) {
    res[i].word = strdup(words[i].first.c_str());
    res[i].weight = words[i].second;
  }
  res[words.size()].word = NULL;
  return res;
}

#ifdef __cplusplus
extern "C" {
#endif

jieba_t* jieba_new(const char* dict_path, const char* hmm_path, const char* user_dict, const char* idf_path, const char* stop_words_path) {
  jieba_t* handle = (jieba_t*)(new cppjieba::Jieba(dict_path, hmm_path, user_dict, idf_path, stop_words_path));
  return handle;
}

void jieba_free(jieba_t* handle) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  delete x;
}

void jieba_reset_separators(jieba_t* handle, const char* sep) {
  ((cppjieba::Jieba*)handle)->ResetSeparators(sep);
}

void jieba_words_free(jieba_words_t* words) {
    for (size_t i = 0; i < words->length; i++) {
        if (words->words[i] != NULL) {
            free(words->words[i]);
        }
    }
    free(words->words);
    free(words);
}

void jieba_token_free(jieba_token_t* tokens) {
    free(tokens);
}

jieba_words_t* jieba_cut(jieba_t* handle, const char* sentence, int is_hmm_used) {
  std::vector<std::string> words;
  ((cppjieba::Jieba*)handle)->Cut(sentence, words, is_hmm_used);
  return convert_words(words);
}

jieba_words_t* jieba_cut_all(jieba_t* handle, const char* sentence) {
  std::vector<std::string> words;
  ((cppjieba::Jieba*)handle)->CutAll(sentence, words);
  return convert_words(words);
}

jieba_words_t* jieba_cut_hmm(jieba_t* handle, const char* sentence) {
  std::vector<std::string> words;
  ((cppjieba::Jieba*)handle)->CutHMM(sentence, words);
  return convert_words(words);
}

jieba_words_t* jieba_cut_for_search(jieba_t* handle, const char* sentence, int is_hmm_used) {
  std::vector<std::string> words;
  ((cppjieba::Jieba*)handle)->CutForSearch(sentence, words, is_hmm_used);
  return convert_words(words);
}

jieba_words_t* jieba_cut_small(jieba_t* handle, const char* sentence, size_t max_word_len) {
  std::vector<std::string> words;
  ((cppjieba::Jieba*)handle)->CutSmall(sentence, words, max_word_len);
  return convert_words(words);
}

jieba_words_t* jieba_tag(jieba_t* handle, const char* sentence) {
  std::vector<std::pair<std::string, std::string> > result;
  ((cppjieba::Jieba*)handle)->Tag(sentence, result);
  std::vector<std::string> words;
  words.reserve(result.size());
  for (size_t i = 0; i < result.size(); ++i) {
    words.push_back(result[i].first + "/" + result[i].second);
  }
  return convert_words(words);
}

char* jieba_lookup_tag(jieba_t* handle, const char* str) {
  std::string tag = ((cppjieba::Jieba*)handle)->LookupTag(str);
  return strdup(tag.c_str());
}

void jieba_str_free(char* str) {
  free(str);
}

void jieba_add_user_word(jieba_t* handle, const char* word) {
  ((cppjieba::Jieba*)handle)->InsertUserWord(word);
}

void jieba_add_user_words(jieba_t* handle, const char** words, size_t count) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  for (size_t i = 0; i < count; i++) {
    x->InsertUserWord(words[i]);
  }
}

jieba_token_t* jieba_tokenize(jieba_t* handle, const char* sentence, jieba_tokenize_mode_t mode, int is_hmm_used) {
  std::vector<cppjieba::Word> words;
  switch (mode) {
    case JIEBA_TOKENIZE_MODE_SEARCH:
      ((cppjieba::Jieba*)handle)->CutForSearch(sentence, words, is_hmm_used);
      return convert_tokens(words);
    default:
      ((cppjieba::Jieba*)handle)->Cut(sentence, words, is_hmm_used);
      return convert_tokens(words);
  }
}

jieba_word_weight_t* jieba_extract_with_weight(jieba_t* handle, const char* sentence, int top_k) {
  std::vector<std::pair<std::string, double> > words;
  ((cppjieba::Jieba*)handle)->extractor.Extract(sentence, words, top_k);
  jieba_word_weight_t* res = convert_word_weights(words);
  return res;
}

jieba_words_t* jieba_extract(jieba_t* handle, const char* sentence, int top_k) {
  std::vector<std::string> words;
  ((cppjieba::Jieba*)handle)->extractor.Extract(sentence, words, top_k);
  return convert_words(words);
}

void jieba_word_weight_free(jieba_word_weight_t* wws) {
  jieba_word_weight_t* x = wws;
  while (x && x->word) {
    free(x->word);
    x->word = NULL;
    x++;
  }
  free(wws);
}

#ifdef __cplusplus
} /* end extern "C" */
#endif
