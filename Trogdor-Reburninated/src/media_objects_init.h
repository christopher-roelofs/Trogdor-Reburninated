#include "config.h"
#include "sprite_objects.h"
#if !defined(SDL1)
#include "text_objects_sdl2.h"
#else
#include "text_objects_sdl1.h"
#endif

#ifndef MEDIA_OBJECTS_INIT_H
#define MEDIA_OBJECTS_INIT_H

extern SDL_Surface *temp;

extern SpriteObject sprite_videlectrix_logo;
extern SpriteObject sprite_title_screen;
extern SpriteObject sprite_trogdor_logo;
extern SpriteObject sprite_level_background_1;
extern SpriteObject sprite_level_background_2;
extern SpriteObject sprite_level_background_3;
extern SpriteObject sprite_level_background_4;
extern SpriteObject sprite_level_background_th;
extern SpriteObject *sprite_level_background;
extern SpriteObject sprite_trogdor;
extern SpriteObject sprite_burnination_meter_full;
extern SpriteObject sprite_burnination_meter_empty;
extern SpriteObject sprite_cottage;
extern SpriteObject sprite_cottage_fire;
extern SpriteObject sprite_peasantometer_icon;
extern Uint8        sprite_peasantometer_icon_init_x;
extern SpriteObject sprite_archer;
extern SpriteObject sprite_arrow;

extern Mix_Chunk *sfx_burn_hut;
extern Mix_Chunk *sfx_goldget;
extern Mix_Chunk *sfx_peasantscream;
extern Mix_Chunk *sfx_sb1;
extern Mix_Chunk *sfx_sb2;
extern Mix_Chunk *sfx_sb3;
extern Mix_Chunk *sfx_sb4;
extern Mix_Chunk *sfx_sb5;
extern Mix_Chunk *sfx_sb6;
extern Mix_Chunk *sfx_sb7;
extern Mix_Chunk *sfx_sbarch;
extern Mix_Chunk *sfx_sbarchend;
extern Mix_Chunk *sfx_sbbest;
extern Mix_Chunk *sfx_sbdooj;
extern Mix_Chunk *sfx_sbgameover;
extern Mix_Chunk *sfx_sbkerrek;
extern Mix_Chunk *sfx_sblevelbeat;
extern Mix_Chunk *sfx_sbscore;
extern Mix_Chunk *sfx_sbsecret;
extern Mix_Chunk *sfx_sbsquish1;
extern Mix_Chunk *sfx_sbsquish2;
extern Mix_Chunk *sfx_sbwin;
extern Mix_Chunk *sfx_sbwin2;
extern Mix_Chunk *sfx_sbworst;
extern Mix_Chunk *sfx_sfx2;
extern Mix_Chunk *sfx_trogador;

extern TTF_Font *font_serif_brown_6;
extern TextCharObject textChars_font_serif_brown_6[122 + 1 - 32];
extern TTF_Font *font_serif_brown_8;
extern TextCharObject textChars_font_serif_brown_8[126 + 1 - 32];
extern TTF_Font *font_serif_gray_6;
extern TextCharObject textChars_font_serif_gray_6[126 + 1 - 32];
extern TTF_Font *font_serif_gray_12;
extern TextCharObject textChars_font_serif_gray_12[90 + 1 - 32];
extern TTF_Font *font_serif_orange_6;
extern TextCharObject textChars_font_serif_orange_6[90 + 1 - 32];
extern TTF_Font *font_serif_red_6;
extern TextCharObject textChars_font_serif_red_6[126 + 1 - 32];
extern TTF_Font *font_serif_red_8;
extern TextCharObject textChars_font_serif_red_8[126 + 1 - 32];
extern TTF_Font *font_serif_red_12;
extern TextCharObject textChars_font_serif_red_12[90 + 1 - 32];
extern TTF_Font *font_serif_white_6;
extern TextCharObject textChars_font_serif_white_6[126 + 1 - 32];
extern TTF_Font *font_serif_white_9;
extern TextCharObject textChars_font_serif_white_9[126 + 1 - 32];
extern TTF_Font *font_serif_white_10;
extern TextCharObject textChars_font_serif_white_10[126 + 1 - 32];
extern TTF_Font *font_serif_white_14;
extern TextCharObject textChars_font_serif_white_14[126 + 1 - 32];
extern TTF_Font *font_nokia_12;
extern TextCharObject textChars_font_nokia_12[126 + 1 - 32];
extern TTF_Font *font_serif_2_bold_black_23;
extern TextCharObject textChars_font_serif_2_bold_black_23[90 + 1 - 32];
extern TTF_Font *font_serif_2_bold_red_23;
extern TextCharObject textChars_font_serif_2_bold_red_23[90 + 1 - 32];
extern TTF_Font *font_serif_2_red_6;
extern TextCharObject textChars_font_serif_2_red_6[90 + 1 - 32];
extern TTF_Font *font_serif_2_red_13;
extern TextCharObject textChars_font_serif_2_red_13[90 + 1 - 32];

extern TextObject text_0_loading;
extern SDL_Rect text_0_loading_censor_rect;
extern TextObject text_1_presents;
extern TextObject text_3_click_anywhere_to_start;
extern TextObject text_3_programmed;
extern TextObject text_3_designed;
extern TextObject text_3_instructions_1;
extern TextObject text_3_instructions_2;
extern TextObject text_3_instructions_3;
extern TextObject text_3_instructions_4;
extern TextObject text_3_instructions_5;
extern TextObject text_3_hints_1;
extern TextObject text_3_hints_2;
extern TextObject text_3_hints_3;
extern TextObject text_3_hints_4;
extern TextObject text_3_hints_5;
extern TextObject text_3_hints_6;
extern TextObject text_3_hints_7;
extern TextObject text_4_score_val;
extern TextObject text_4_mans_val;
extern TextObject text_4_level_val;
extern TextObject text_4_score;
extern TextObject text_4_mans;
extern TextObject text_4_level;
extern TextObject text_4_burninate_black;
extern TextObject text_4_burninate_red;
extern TextObject text_6_paused_1;
extern TextObject text_6_paused_2;
extern TextObject text_9_level_gray;
extern TextObject text_9_beaten_gray;
extern TextObject text_9_level_red;
extern TextObject text_9_beaten_red;
extern TextObject text_9_nice_work;
extern TextObject text_10_again_gray;
extern TextObject text_10_challenge_gray;
extern TextObject text_10_again_white;
extern TextObject text_10_challenge_white;
extern TextObject text_10_again_red;
extern TextObject text_10_challenge_red;
extern TextObject text_10_again_orange;
extern TextObject text_10_challenge_orange;
extern TextObject text_10_its_over_gray;
extern TextObject text_10_its_over_red;
extern TextObject text_10_username_val;
extern TextObject text_10_enter_initials;
extern TextObject text_10_send_em;
extern TextObject text_10_hi_scores_gray;
extern TextObject text_10_view_gray;
extern TextObject text_10_hi_scores_white;
extern TextObject text_10_view_white;
extern TextObject text_10_hi_scores_red;
extern TextObject text_10_view_red;
extern TextObject text_10_hi_scores_orange;
extern TextObject text_10_view_orange;
extern TextObject text_11_cutscene;
extern TextObject text_12_cutscene;
extern TextObject text_13_cutscene;
extern TextObject text_14_cutscene;
extern TextObject text_15_cutscene;
extern TextObject text_16_cutscene;
extern TextObject text_17_cutscene;
extern TextObject text_18_cutscene;
extern TextObject text_19_cutscene;
extern TextObject text_20_cutscene;
extern TextObject text_21_cutscene;
extern TextObject text_22_cutscene;
extern TextObject text_23_congratulations;
extern TextObject text_23_you_got;
extern TextObject text_23_good_score;
extern TextObject text_23_cast;
extern TextObject text_23_trogdor;
extern TextObject text_23_perez;
extern TextObject text_23_hackworth;
extern TextObject text_23_the_steve;
extern TextObject text_23_the_blue_knight;
extern TextObject text_23_the_red_knight;
extern TextObject text_23_the_siamese;
extern TextObject text_23_archers;
extern TextObject text_23_and_wordly_wise;
extern TextObject text_23_as_the_kerrek;
extern TextObject text_23_keep_playing;
extern TextObject text_25_ye_olde_high_scores;
extern TextObject text_25_name;
extern TextObject text_25_level;
extern TextObject text_25_score;
extern TextObject text_25_name_1_val;
extern TextObject text_25_level_1_val;
extern TextObject text_25_score_1_val;
extern TextObject text_25_1;
extern TextObject text_25_name_2_val;
extern TextObject text_25_level_2_val;
extern TextObject text_25_score_2_val;
extern TextObject text_25_2;
extern TextObject text_25_name_3_val;
extern TextObject text_25_level_3_val;
extern TextObject text_25_score_3_val;
extern TextObject text_25_3;
extern TextObject text_25_name_4_val;
extern TextObject text_25_level_4_val;
extern TextObject text_25_score_4_val;
extern TextObject text_25_4;
extern TextObject text_25_name_5_val;
extern TextObject text_25_level_5_val;
extern TextObject text_25_score_5_val;
extern TextObject text_25_5;
extern TextObject text_25_name_6_val;
extern TextObject text_25_level_6_val;
extern TextObject text_25_score_6_val;
extern TextObject text_25_6;
extern TextObject text_25_name_7_val;
extern TextObject text_25_level_7_val;
extern TextObject text_25_score_7_val;
extern TextObject text_25_7;
extern TextObject text_25_name_8_val;
extern TextObject text_25_level_8_val;
extern TextObject text_25_score_8_val;
extern TextObject text_25_8;
extern TextObject text_25_name_9_val;
extern TextObject text_25_level_9_val;
extern TextObject text_25_score_9_val;
extern TextObject text_25_9;
extern TextObject text_25_name_10_val;
extern TextObject text_25_level_10_val;
extern TextObject text_25_score_10_val;
extern TextObject text_25_10;

#endif