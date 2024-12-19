// Copyright 2024 Bernie Liu
// Author: Bernie Liu
// UWNetID: bliu29
// Homework 5
// 6/11/2024
// Tests the functions of t9_lib.h

#include <string.h>
#include <stdio.h>

#include "safe_assert.h"
#include "t9_lib.h"

void AssertReturnedStringEquals(char* expected, char* actual);

suite("T9") {
  test("Empty initialization") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);
    DestroyT9(dict);
  }

  test("Add words to empty initialization and check words of same pattern") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");

    char* word = PredictT9(dict, "2665#");
    
    AssertReturnedStringEquals("cool", word);

    DestroyT9(dict);
  }

  test("Initialize from dictionary.txt") {
    char* filename="dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    char* word=PredictT9(dict, "22");

    AssertReturnedStringEquals("aa", word);

    DestroyT9(dict);
  }

  test("Initialize from nonexistent file"){
    char* filename="nonexistent.txt";
    safe_assert(InitializeFromFileT9(filename)== NULL);

   
  }

  test("Predict word not in dictionary") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);

    safe_assert(dict != NULL);

    char* word = PredictT9(dict, "222");
    safe_assert(word==NULL);

    DestroyT9(dict);
  }

  test("Check T9 accuracy") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "adz");
    AddWordToT9(dict, "pqrs");
    AddWordToT9(dict, "wxyz");

    char* word=PredictT9(dict, "239");
    AssertReturnedStringEquals("adz", word);

    char* pqrs=PredictT9(dict, "7777");
    AssertReturnedStringEquals("pqrs", pqrs);

    char* wxyz=PredictT9(dict, "9999");
    AssertReturnedStringEquals("wxyz", wxyz);

    DestroyT9(dict);
  }

  test("Add same word twice") {
    T9* dict1 = InitializeEmptyT9();
    safe_assert(dict1 != NULL);

    AddWordToT9(dict1, "book");
    
    T9* dict2 = InitializeEmptyT9();
    safe_assert(dict2 != NULL);

    AddWordToT9(dict2, "book");
    AddWordToT9(dict2, "book");

    char* word=PredictT9(dict2, "2665#");
    safe_assert(word==NULL);
    safe_assert(sizeof(dict1)==sizeof(dict2));

    DestroyT9(dict1);
    DestroyT9(dict2);
    
  }

  test("Same numerical patterns") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "aa");
    AddWordToT9(dict, "ab");
    AddWordToT9(dict, "ac");
    AddWordToT9(dict, "ba");
    AddWordToT9(dict, "bb");
    AddWordToT9(dict, "bc");

    char* aa=PredictT9(dict, "22");
    char* ab=PredictT9(dict, "22#");
    char* ac=PredictT9(dict, "22##");
    char* ba=PredictT9(dict, "22###");
    char* bb=PredictT9(dict, "22####");
    char* bc=PredictT9(dict, "22#####");

    AssertReturnedStringEquals(aa, "aa");
    AssertReturnedStringEquals(ab, "ab");
    AssertReturnedStringEquals(ac, "ac");
    AssertReturnedStringEquals(ba, "ba");
    AssertReturnedStringEquals(bb, "bb");
    AssertReturnedStringEquals(bc, "bc");

    DestroyT9(dict);
  }

  test("Add empty string") {
    T9* dict1 = InitializeEmptyT9();
    safe_assert(dict1 != NULL);
    
    T9* dict2 = InitializeEmptyT9();
    safe_assert(dict2 != NULL);

    AddWordToT9(dict2, "");
    safe_assert(sizeof(dict1)==sizeof(dict2));
   
    DestroyT9(dict1);
    DestroyT9(dict2);
  }

  test("Add empty string or uppercase word or numbers") {
    T9* dict1 = InitializeEmptyT9();
    safe_assert(dict1 != NULL);
    
    T9* dict2 = InitializeEmptyT9();
    safe_assert(dict2 != NULL);

    AddWordToT9(dict2, "AAA");
    safe_assert(sizeof(dict1)==sizeof(dict2));

    AddWordToT9(dict2, "AaA");
    safe_assert(sizeof(dict1)==sizeof(dict2));

    DestroyT9(dict1);
    DestroyT9(dict2);
  }

  test("Add numbers") {
    T9* dict1 = InitializeEmptyT9();
    safe_assert(dict1 != NULL);
    
    T9* dict2 = InitializeEmptyT9();
    safe_assert(dict2 != NULL);

    AddWordToT9(dict2, "123");
    safe_assert(sizeof(dict1)==sizeof(dict2));
    

    DestroyT9(dict1);
    DestroyT9(dict2);
  }

  test("Add substring of string") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "run");
    AddWordToT9(dict, "running");

    AddWordToT9(dict, "gold");
    AddWordToT9(dict, "goldfish");
    AddWordToT9(dict, "go");
    AddWordToT9(dict, "fish");

    char* gold=PredictT9(dict, "4653");
    char* go=PredictT9(dict, "46");
    char* goldfish=PredictT9(dict, "46533474");
    char* fish=PredictT9(dict, "3474");
    char* run=PredictT9(dict, "786");
    char* running=PredictT9(dict, "7866464");

    AssertReturnedStringEquals("gold", gold);
    AssertReturnedStringEquals("goldfish", goldfish);
    AssertReturnedStringEquals("fish", fish);
    AssertReturnedStringEquals("go", go);
    AssertReturnedStringEquals("run", run);
    AssertReturnedStringEquals("running", running);

    DestroyT9(dict);
  }

  test("Predict first and last of small_dictionary.txt") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    char* aardvark=PredictT9(dict, "22738275");
    AssertReturnedStringEquals("aardvark", aardvark);

    char* zebra=PredictT9(dict, "93272");
    AssertReturnedStringEquals("zebra", zebra);

    DestroyT9(dict);

  }

  test("PredictT9 NULL returns NULL") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    safe_assert(PredictT9(dict, NULL)== NULL);
    

    DestroyT9(dict);
  }

  test("PredictT9 \"\" returns NULL") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    safe_assert(PredictT9(dict, "")==NULL);

    DestroyT9(dict);
  }

  test("PredictT9 1 returns NULL") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);
    safe_assert(PredictT9(dict, "1")==NULL);

    DestroyT9(dict);
  }

  test("PredictT9 # followed by numbers returns NULL") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    safe_assert(PredictT9(dict, "#93272")==NULL);

    DestroyT9(dict);
  }

  test("PredictT9 # in middle of numbers returns NULL") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    safe_assert(PredictT9(dict, "93#272")==NULL);

    DestroyT9(dict);
  }

  test("PredictT9 more # than available words from pattern returns NULL") {
    char* filename="small_dictionary.txt";
    T9* dict=InitializeFromFileT9(filename);
    safe_assert(dict != NULL);

    safe_assert(PredictT9(dict, "93272#")==NULL);

    DestroyT9(dict);
  }

  test("Add whitespace to T9") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    T9* empty = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, " ");

    safe_assert(sizeof(dict)==sizeof(empty));

    DestroyT9(dict);
    DestroyT9(empty);
  }

  test("Add new line to T9") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    T9* empty = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "\n");

    safe_assert(sizeof(dict)==sizeof(empty));

    DestroyT9(dict);
    DestroyT9(empty);
  }

  test("Add \"\" to T9") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    T9* empty = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "");
    AddWordToT9(dict, "\"cool\"");

    safe_assert(sizeof(dict)==sizeof(empty));
    safe_assert(PredictT9(dict, "2665")==NULL)

    DestroyT9(dict);
    DestroyT9(empty);
  }

  test("Add punctuation to T9") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    T9* empty = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "?");

    safe_assert(sizeof(dict)==sizeof(empty));

    AddWordToT9(dict, "!");

    safe_assert(sizeof(dict)==sizeof(empty));

    AddWordToT9(dict, ";");

    safe_assert(sizeof(dict)==sizeof(empty));

    AddWordToT9(dict, ",");

    safe_assert(sizeof(dict)==sizeof(empty));

    AddWordToT9(dict, ".");

    safe_assert(sizeof(dict)==sizeof(empty));

    AddWordToT9(dict, "#");

    safe_assert(sizeof(dict)==sizeof(empty));

    DestroyT9(dict);
    DestroyT9(empty);
  }



  test("Check if middle word is added from file") {
    T9* dict1 = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict1 != NULL);
    char* book=PredictT9(dict1, "2665");
    AssertReturnedStringEquals("book", book);
    DestroyT9(dict1);
  }


}

void AssertReturnedStringEquals(char* expected, char* actual) {
  safe_assert(actual > 0);
  safe_assert(strlen(actual) == strlen(expected));
  safe_assert(strcmp(actual, expected) == 0);
}
