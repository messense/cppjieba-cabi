#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "jieba.h"

const char* DICT_PATH = "./cppjieba/dict/jieba.dict.utf8";
const char* HMM_PATH = "./cppjieba/dict/hmm_model.utf8";
const char* USER_DICT = "./cppjieba/dict/user.dict.utf8";
const char* IDF_PATH = "./cppjieba/dict/idf.utf8";
const char* STOP_WORDS_PATH = "./cppjieba/dict/stop_words.utf8";

void CutDemo() {
  printf("CutDemo:\n");
  // init will take a few seconds to load dicts.
  jieba_t* handle = jieba_new(DICT_PATH, HMM_PATH, USER_DICT, IDF_PATH, STOP_WORDS_PATH);

  const char* s = "南京市长江大桥";
  jieba_words_t* words = jieba_cut(handle, s, 1);
  for (size_t i = 0; i < words->length; i++) {
    char* word = words->words[i];
    printf("%s\n", word);
  }
  jieba_words_free(words);
  jieba_free(handle);
}

void CutAllDemo() {
  printf("CutAllDemo:\n");
  // init will take a few seconds to load dicts.
  jieba_t* handle = jieba_new(DICT_PATH, HMM_PATH, USER_DICT, IDF_PATH, STOP_WORDS_PATH);

  const char* s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";
  jieba_words_t* words = jieba_cut_all(handle, s);
  for (size_t i = 0; i < words->length; i++) {
    char* word = words->words[i];
    printf("%s\n", word);
  }
  jieba_words_free(words);
  jieba_free(handle);
}

void UserWordDemo()
{
  printf("UserWordDemo:\n");
  jieba_t* handle = jieba_new(DICT_PATH, HMM_PATH, USER_DICT, IDF_PATH, STOP_WORDS_PATH);

  const char* s = "人艰不拆";

  jieba_words_t* words = jieba_cut(handle, s, 1);
  for (size_t i = 0; i < words->length; i++) {
    char* word = words->words[i];
    printf("%s\n", word);
  }
  jieba_words_free(words);

  jieba_add_user_word(handle, "人艰不拆");
  words = jieba_cut(handle, s, 1);
  for (size_t i = 0; i < words->length; i++) {
    char* word = words->words[i];
    printf("%s\n", word);
  }
  jieba_words_free(words);
  jieba_free(handle);
}

int main(int argc, char** argv) {
  CutDemo();
  CutAllDemo();
  UserWordDemo();
  return 0;
}
