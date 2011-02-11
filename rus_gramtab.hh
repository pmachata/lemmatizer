#ifndef RUS_GRAMTAB_HH
#define RUS_GRAMTAB_HH

enum pos_code_t
  {
    pos__invalid = -1,
    pos_noun = 0,
    pos_adjective = 1,
    pos_verb = 2,
    pos_pronoun = 3,
    pos_4 = 4, //"МС-П"
    pos_5 = 5, //"МС-ПРЕДК"
    pos_6 = 6, //"ЧИСЛ"
    pos_7 = 7, //"ЧИСЛ-П"
    pos_adverb = 8, //"Н", // 8
    pos_9 = 9, //"ПРЕДК"
    pos_10 = 10, //"ПРЕДЛ"
    pos_11 = 11, //"ПОСЛ"
    pos_conjunction = 12,
    pos_interjection = 13,
    pos_transition_word = 14,
    pos_15 = 15, //"ФРАЗ"
    pos_particle = 16,
    pos_short_adjective = 17,
    pos_adj_participle = 18, //"ПРИЧАСТИЕ"
    pos_adv_participle = 19, //"ДЕЕПРИЧАСТИЕ"
    pos_short_participle = 20,
    pos_infinitive = 21,
  };

enum gram_code_t
  {
    gm__invalid = -1,
    gm_plural = 0,
    gm_singular = 1,
    gm_nominative = 2,
    gm_genitive = 3,
    gm_dative = 4,
    gm_accusative = 5,
    gm_instrumental = 6,
    gm_prepositional = 7,
    gm_vocative = 8,
    gm_masculine = 9,
    gm_feminine = 10,
    gm_neuter = 11,
    gm_masc_femin = 12,
    gm_present = 13,
    gm_future = 14,
    gm_past = 15,
    gm_1st_person = 16,
    gm_2nd_person = 17,
    gm_3rd_person = 18,
    gm_imperative = 19,
    gm_animate = 20,
    gm_inanimate = 21,
    gm_comparative = 22,
    gm_perfective = 23,
    gm_imperfective = 24,
    gm_25 = 25, //"нп"
    gm_26 = 26, //"пе"
    gm_active = 27,
    gm_passive = 28,
    gm_0 = 29, // "0"
    gm_abbr = 30, //"аббр"
    gm_patronymic = 31, //"отч" ???
    gm_32 = 32, //"лок"
    gm_33 = 33, //"орг"
    gm_34 = 34, //"кач"
    gm_35 = 35, //"дфст"
    gm_36 = 36, // (наречия) "вопр"
    gm_37 = 37, // (наречия) "указат"
    gm_first_name = 38,
    gm_surname = 39,
    gm_impersonal = 40, //"безл" ???
    gm_jargon = 41, //"жарг"
    gm_42 = 42, //"опч"
    gm_colloquial = 43, //"разг"
    gm_44 = 44, //"притяж"
    gm_45 = 45, //"арх"
    gm_secondary = 46, // для второго родительного и второго предложного
    gm_poetic = 47,
    gm_prof = 48,
    gm_superlative = 49, //"прев"
    gm_positive = 50, //"полож"
  };

// If CODE has information about number, store it in ret and return
// true.  If not, leave it intact and return false.
bool extract_rus_number (gram_code_t &ret, gram_code_t code);
bool extract_rus_case (gram_code_t &ret, gram_code_t code);
bool extract_rus_gender (gram_code_t &ret, gram_code_t code);
bool extract_rus_adj_degree (gram_code_t &ret, gram_code_t code);
bool extract_rus_voice (gram_code_t &ret, gram_code_t code);
bool extract_rus_animacy (gram_code_t &ret, gram_code_t code);
bool extract_rus_tense (gram_code_t &ret, gram_code_t code);
bool extract_rus_person (gram_code_t &ret, gram_code_t code);

#endif//RUS_GRAMTAB_HH
