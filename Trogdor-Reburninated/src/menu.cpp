#include "menu.h"
#include "input.h"
#include "general.h"
#include "window.h"

Sint8 currOnscreenIndex;
Sint8 counter;
bool menusAreInitialized = false;
FontObject *menuFont;

#define CURR_OPTION options[cursorIndex]
#define CURR_OPTION_ONSCREEN options[currOnscreenIndex]
#define CURR_SPACER_X (spacer_x * (currOnscreenIndex - topOnscreenIndex))

#define CENTER_X(textObj)  OBJ_TO_MID_SCREEN_X((Sint16)(appWidth * screenScale_menu), textObj)
#define DESC_LINE_Y_TOP    (Sint16)(start_y_desc * screenScale_menu)
#define DESC_LINE_Y_UPPER  (Sint16)((start_y_desc + (spacer_y_desc / 2)) * screenScale_menu)
#define DESC_LINE_Y_MID    (Sint16)((start_y_desc + spacer_y_desc) * screenScale_menu)
#define DESC_LINE_Y_LOWER  (Sint16)((start_y_desc + (spacer_y_desc * 3 / 2)) * screenScale_menu)
#define DESC_LINE_Y_BOTTOM (Sint16)((start_y_desc + (spacer_y_desc * 2)) * screenScale_menu)

/*************/
/* MENU PAGE */
/*************/

void MenuPage::prepareMenuPage(Sint8 num_lines, Sint16 st_x, Sint16 st_y, Sint16 sp_y, Sint8 align_type) {
	numLines = num_lines;
	start_x = st_x;
	start_y = sp_y;
	spacer_y = sp_y;
	alignType = align_type;
}

void MenuPage::setTextLine(Sint8 index, const char text[]) {
	setText(text, &lines[index], menuFont);
	switch (alignType) {
		case 0:
			lines[index].dstrect.x = start_x;
			break;
		case 1:
			lines[index].dstrect.x = CENTER_X(lines[index]);
			break;
		default:
			lines[index].dstrect.x = start_x - lines[index].dstrect.w;
			break;
	}
	lines[index].dstrect.y = (Sint16)((start_y + (spacer_y * index)) * screenScale_menu);
}

void MenuPage::render() {
	for (counter = 0; counter < numLines; counter++) {
		renderText_menu(lines[counter], *menuFont);
	}
}

/*****************/
/* MENU NOTEBOOK */
/*****************/

void MenuNotebook::prepareMenuNotebook(Sint8 num_pages, Sint16 pos_x, Sint16 pos_y, Sint8 align_type) {
	numPages = num_pages;
	pageCounter.dstrect.y = (Sint16)(pos_y * screenScale_menu);
	setText("(1/5)", &pageCounter, menuFont);
	pageCounter_x = (Sint16)(pos_x * screenScale_menu);
	alignType_pageCounter = align_type;
	if (!menusAreInitialized) {
		index = 0;
	}
	updatePageCounterText();
}

void MenuNotebook::openNotebook() {
	index = 0;
	updatePageCounterText();
}

void MenuNotebook::updatePageCounterText() {
	updateText(&pageCounter, "(" + to_string((index + 1)) + "/" + to_string(numPages) + ")");
	switch (alignType_pageCounter) {
		case 0:
			pageCounter.dstrect.x = pageCounter_x;
			break;
		case 1:
			pageCounter.dstrect.x = pageCounter_x - (pageCounter.dstrect.w / 2);
			break;
		default:
			pageCounter.dstrect.x = pageCounter_x - pageCounter.dstrect.w;
			break;
	}
}

Sint8 MenuNotebook::handleInput() {
	if (keyPressed(INPUT_LEFT)) {
		decrementPageNum();
	}
	if (keyPressed(INPUT_RIGHT)) {
		incrementPageNum();
	}
	if (keyPressed(INPUT_A)) {
		return index;
	}
	if (keyPressed(INPUT_B) || keyPressed(INPUT_SELECT)) {
		return -1;
	}
	return -2;
}

void MenuNotebook::decrementPageNum() {
	if (index > 0) {
		index--;
		updatePageCounterText();
	}
}

void MenuNotebook::incrementPageNum() {
	if (index < numPages - 1) {
		index++;
		updatePageCounterText();
	}
}

void MenuNotebook::renderNotebook() {
	pages[index]->render();
	renderText_menu(pageCounter, *menuFont);
}

/***************/
/* MENU OPTION */
/***************/

void MenuOption::prepareMenuOption(const char label_ptr[], std::string choice_ptr[], std::string desc_ptr_1[], std::string desc_ptr_2[], std::string desc_ptr_3[], std::string altDesc_ptr, Uint8 numCh, bool oneDesc, Sint8 start, bool wrap) {
	labelPtr = label_ptr;
	if (!menusAreInitialized) {
		choicePtr = choice_ptr;
		descPtr_1 = desc_ptr_1;
		descPtr_2 = desc_ptr_2;
		descPtr_3 = desc_ptr_3;
		altDescPtr = altDesc_ptr;
		numChoices = numCh;
		oneDescription = oneDesc;
		for (i = 0; i < numChoices; i++) {
			choiceIsAllowed[i] = true;
		}
		index = start;
		index_init = start;
		choicesWrap = wrap;
		optionIsLocked = false;
	}
	updateLabel();
	initChoicesAndDescriptions();
	updateChoice();
	updateDescription();
}

void MenuOption::setLocked(bool locked) {
	optionIsLocked = locked;
	updateLabel();
	updateChoice();
	updateDescription();
}

void MenuOption::updateLabel() {
	if (!optionIsLocked) {
		if (!labelPtr.empty()) {
			setText(labelPtr, &label, menuFont);
		}
	} else {
		setText("???", &label, menuFont);
	}
}

// this initializes text chars used in choices that aren't initially selected
void MenuOption::initChoicesAndDescriptions() {
	if (choicePtr != NULL) {
		for (counter = 0; counter < numChoices; counter++) {
			setText(choicePtr[counter], &choice, menuFont);
		}
	}
	if (descPtr_1 != NULL) {
		if (!oneDescription) {
			for (counter = 0; counter < numChoices; counter++) {
				setText(descPtr_1[counter], &description_1, menuFont);
				setText(descPtr_2[counter], &description_1, menuFont);
				setText(descPtr_3[counter], &description_1, menuFont);
			}
		}
	}
}

void MenuOption::updateChoice() {
	if (!optionIsLocked) {
		if (choicePtr != NULL) {
			setText(choicePtr[index], &choice, menuFont);
		}
	} else {
		setText("", &choice, menuFont);
	}
}

void MenuOption::updateDescription() {
	if (!optionIsLocked) {
		if (descPtr_1 != NULL) {
			if (oneDescription) {
				setText(descPtr_1[0], &description_1, menuFont);
				setText(descPtr_2[0], &description_2, menuFont);
				setText(descPtr_3[0], &description_3, menuFont);
			} else {
				setText(descPtr_1[index], &description_1, menuFont);
				setText(descPtr_2[index], &description_2, menuFont);
				setText(descPtr_3[index], &description_3, menuFont);
			}
		}
	} else {
		if (!altDescPtr.empty()) {
			setText(altDescPtr, &description_1, menuFont);
			setText("", &description_2, menuFont);
			setText("", &description_3, menuFont);
		}
	}
}

void MenuOption::setDescriptionToIndex(Uint8 forcedIndex) {
	setText(descPtr_1[forcedIndex], &description_1, menuFont);
	setText(descPtr_2[forcedIndex], &description_2, menuFont);
	setText(descPtr_3[forcedIndex], &description_3, menuFont);
}

void MenuOption::render(bool renderDescription) {
	renderText_menu(label, *menuFont);
	renderText_menu(choice, *menuFont);
	if (renderDescription) {
		renderText_menu(description_1, *menuFont);
		renderText_menu(description_2, *menuFont);
		renderText_menu(description_3, *menuFont);
	}
}

bool MenuOption::isValue(Uint8 val) {
	return (!optionIsLocked && index == val);
}

/********/
/* MENU */
/********/

void Menu::prepareMenu(Uint8 numOpt, Uint8 numOns, SpriteObject *spriteObj, bool keepIndex, Sint8 space_scroll,
	Sint16 st_x_label, Sint16 st_x_choice, Sint16 sp_x, Sint16 st_y_option, Sint16 st_y_desc, Sint16 sp_y_option, Sint16 sp_y_desc,
	Sint8 at_label, Sint8 at_choice, bool wrap) {
	start_x_label = st_x_label;
	start_x_choice = st_x_choice;
	spacer_x = sp_x;
	start_y_option = st_y_option;
	start_y_desc = st_y_desc;
	spacer_y_option = sp_y_option;
	spacer_y_desc = sp_y_desc;
	alignType_label = at_label;
	alignType_choice = at_choice;
	cursor = SpriteInstance(spriteObj, 0, 0, spriteObj->dstrect.x, spriteObj->dstrect.y);
	if (!menusAreInitialized) {
		numOptions = numOpt;
		numOnscreen = min(numOns, numOptions);
		cursorIndex = 0;
		scrollIndex = 0;
		cursorIndex_onscreen = 0;
		keepIndexOnExit = keepIndex;
		scrollSpacer = space_scroll;
		topOnscreenIndex = 0;
		bottomOnscreenIndex = 0;
		optionsWrap = wrap;
	}
}

Sint8 Menu::handleInput() {
	if (keyPressed(INPUT_UP)) {
		decrementOption();
	}
	if (keyPressed(INPUT_DOWN)) {
		incrementOption();
	}
	if (keyPressed(INPUT_LEFT)) {
		decrementCurrOptionChoice();
	}
	if (keyPressed(INPUT_RIGHT)) {
		incrementCurrOptionChoice();
	}
	if (keyPressed(INPUT_A) || keyPressed(INPUT_START)) {
		return cursorIndex;
	}
	if (keyPressed(INPUT_B) || keyPressed(INPUT_SELECT)) {
		return -1;
	}
	return -2;
}

void Menu::incrementOption() {
	if (cursorIndex < numOptions - 1) {
		cursorIndex++;
		cursorIndex_onscreen++;
		if ((cursorIndex_onscreen >= numOnscreen - scrollSpacer) && (scrollIndex < numOptions - numOnscreen)) {
			scrollIndex++;
			cursorIndex_onscreen--;
		}
		updateOptionPositions();
	} else if (optionsWrap) {
		cursorIndex = 0;
		scrollIndex = 0;
		cursorIndex_onscreen = 0;
		updateOptionPositions();
	}
}

void Menu::decrementOption() {
	if (cursorIndex > 0) {
		cursorIndex--;
		cursorIndex_onscreen--;
		if ((cursorIndex_onscreen < scrollSpacer) && (scrollIndex > 0)) {
			scrollIndex--;
			cursorIndex_onscreen++;
		}
		updateOptionPositions();
	} else if (optionsWrap) {
		cursorIndex = numOptions - 1;
		scrollIndex = numOptions - numOnscreen;
		cursorIndex_onscreen = numOnscreen - 1;
		updateOptionPositions();
	}
}

// This assumes at least one choice is allowed
void Menu::incrementCurrOptionChoice() {
	if (CURR_OPTION->choicesWrap) {
		do {
			CURR_OPTION->index = (CURR_OPTION->index + 1) % CURR_OPTION->numChoices;
		} while (!CURR_OPTION->choiceIsAllowed[CURR_OPTION->index]);
	} else if (CURR_OPTION->index < CURR_OPTION->numChoices - 1) {
		do {
			CURR_OPTION->index++;
		} while (CURR_OPTION->index < CURR_OPTION->numChoices - 1 && !CURR_OPTION->choiceIsAllowed[CURR_OPTION->index]);
	}
	CURR_OPTION->updateChoice();
	CURR_OPTION->updateDescription();
	updateOptionChoicePosition(cursorIndex);
}

// This assumes at least one choice is allowed
void Menu::decrementCurrOptionChoice() {
	if (CURR_OPTION->choicesWrap) {
		do {
			CURR_OPTION->index--;
			if (CURR_OPTION->index < 0) {
				CURR_OPTION->index += CURR_OPTION->numChoices;
			}
		} while (!CURR_OPTION->choiceIsAllowed[CURR_OPTION->index]);
	} else if (!CURR_OPTION->choiceIsAllowed[CURR_OPTION->index]){
		do {
			CURR_OPTION->index--;
		} while (CURR_OPTION->index > 0 && !CURR_OPTION->choiceIsAllowed[CURR_OPTION->index]);
	}
	CURR_OPTION->updateChoice();
	CURR_OPTION->updateDescription();
	updateOptionChoicePosition(cursorIndex);
}

void Menu::setOptionChoice(Sint8 optionIndex, Sint8 choiceIndex) {
	options[optionIndex]->index = choiceIndex;
	options[optionIndex]->updateChoice();
	options[optionIndex]->updateDescription();
	updateOptionChoicePosition(optionIndex);
}

void Menu::updateOptionPositions() {
	topOnscreenIndex = cursorIndex - cursorIndex_onscreen;
	bottomOnscreenIndex = min(topOnscreenIndex + numOnscreen, (int)numOptions) - 1;
	for (currOnscreenIndex = topOnscreenIndex; currOnscreenIndex <= bottomOnscreenIndex; currOnscreenIndex++) {
		// Update Label 
		switch (alignType_label) {
			case 0:
				CURR_OPTION_ONSCREEN->label.dstrect.x = start_x_label + CURR_SPACER_X;
				break;
			case 1:
				CURR_OPTION_ONSCREEN->label.dstrect.x = start_x_label + CURR_SPACER_X - (CURR_OPTION_ONSCREEN->label.dstrect.w / 2);
				break;
			default:
				CURR_OPTION_ONSCREEN->label.dstrect.x = start_x_label + CURR_SPACER_X - CURR_OPTION_ONSCREEN->label.dstrect.w;
				break;
		}
		CURR_OPTION_ONSCREEN->label.dstrect.y = start_y_option + (spacer_y_option * (currOnscreenIndex - topOnscreenIndex));

		CURR_OPTION_ONSCREEN->label.dstrect.x = (Sint16)(CURR_OPTION_ONSCREEN->label.dstrect.x * screenScale_menu);
		CURR_OPTION_ONSCREEN->label.dstrect.y = (Sint16)(CURR_OPTION_ONSCREEN->label.dstrect.y * screenScale_menu);
		updateOptionChoicePosition(currOnscreenIndex);
	}
	cursor.dstrect.x = CURR_OPTION->label.dstrect.x - (Sint16)(cursor.dstrect.w * screenScale_menu * 2);
	cursor.dstrect.y = CURR_OPTION->label.dstrect.y + ((CURR_OPTION->label.dstrect.h - (Sint16)(cursor.dstrect.h * screenScale_menu)) / 2);
}

void Menu::updateOptionChoicePosition(Sint8 optionIndex) {
	switch (alignType_choice) {
		case 0:
			options[optionIndex]->choice.dstrect.x = start_x_choice + CURR_SPACER_X;
			break;
		case 1:
			options[optionIndex]->choice.dstrect.x = start_x_choice + CURR_SPACER_X - (CURR_OPTION->choice.dstrect.w / 2);
			break;
		case 2:
			options[optionIndex]->choice.dstrect.x = start_x_choice + CURR_SPACER_X - CURR_OPTION->choice.dstrect.w;
			break;
	}
	options[optionIndex]->choice.dstrect.y = options[optionIndex]->label.dstrect.y;

	options[optionIndex]->choice.dstrect.x = (Sint16)(options[optionIndex]->choice.dstrect.x * screenScale_menu);
	updateOptionDescPosition(optionIndex);
}

void Menu::updateOptionDescPosition(Sint8 optionIndex) {
	if (options[optionIndex]->description_2.dstrect.w == 0) {
		setTextPos(&options[optionIndex]->description_1, CENTER_X(options[optionIndex]->description_1), DESC_LINE_Y_MID);
	} else if (options[optionIndex]->description_3.dstrect.w == 0) {
		setTextPos(&options[optionIndex]->description_1, CENTER_X(options[optionIndex]->description_1), DESC_LINE_Y_UPPER);
		setTextPos(&options[optionIndex]->description_2, CENTER_X(options[optionIndex]->description_2), DESC_LINE_Y_LOWER);
	} else {
		setTextPos(&options[optionIndex]->description_1, CENTER_X(options[optionIndex]->description_1), DESC_LINE_Y_TOP);
		setTextPos(&options[optionIndex]->description_2, CENTER_X(options[optionIndex]->description_2), DESC_LINE_Y_MID);
		setTextPos(&options[optionIndex]->description_3, CENTER_X(options[optionIndex]->description_3), DESC_LINE_Y_BOTTOM);
	}
}

void Menu::openMenu() {
	if (!keepIndexOnExit) {
		cursorIndex = 0;
		scrollIndex = 0;
		cursorIndex_onscreen = 0;
	}
	updateOptionPositions();
}

void Menu::renderMenu() {
	for (i = topOnscreenIndex; i <= bottomOnscreenIndex; i++) {
		options[i]->render(cursorIndex == i);
	}
	cursor.renderSprite_menu();
}

/***********************/
/* MENU INITIALIZATION */
/***********************/

std::string option_on_off[2] = { "On", "Off" };
std::string option_empty[1] = { "" };
std::string option_main_starting_level_choices[10] = { "1", "11", "21", "31", "41", "51", "61", "71", "81", "91" };
std::string option_main_starting_level_descriptions_line_1[1] = { "The level that the game starts on." };
std::string option_main_extra_lives_choices[9] = { "0", "1", "2", "3", "4", "5", "10", "20", "30" };
std::string option_main_extra_lives_descriptions_line_1[1] = { "The number of extra lives you start with." };
std::string option_main_lives_interval_choices[8] = { "Every 300 points", "Every 500 points", "Every 1000 points", "500 points only", "500 and 1000 only", "1000 points only", "1000 and 2000 only", "None" };
std::string option_main_lives_interval_descriptions_line_1[8] = { "Get an extra life", "Get an extra life", "Get an extra life", "Get an extra life", "Get an extra life", "Get an extra life", "Get an extra life", "No extra lives." };
std::string option_main_lives_interval_descriptions_line_2[8] = { "every 300 points.", "every 500 points.", "every 1000 points.", "at 500 points.", "at 500 and 1000 points.", "at 1000 points.", "at 1000 and 2000 points.", "" };
std::string option_main_lives_interval_descriptions_line_3[8] = { "", "", "", "", "", "", "", "" };
std::string option_main_music_choices[2] = { "Off", "Stinkoman 20X6"};
std::string option_main_music_descriptions_line_1[2] = { "Don't play music in-game.", "Play music from Stinkoman 20X6." };
std::string option_main_music_descriptions_line_2[2] = { "", "The song changes depending on the level." };
std::string option_main_music_descriptions_line_3[2] = { "", "" };
std::string option_main_peasant_penalty_descriptions_line_1[2] = { "The peasant meter decreases", "No penalty for escaped peasants." };
std::string option_main_peasant_penalty_descriptions_line_2[2] = { "every time a peasant escapes.", "" };
std::string option_main_peasant_penalty_descriptions_line_3[2] = { "", "" };
std::string option_main_treasure_huts_choices[3] = { "New", "Original", "Off" };
std::string option_main_treasure_huts_descriptions_line_1[3] = { "You can enter all treasure huts.", "After you enter a treasure hut, you cannot", "Treasure huts are disabled."};
std::string option_main_treasure_huts_descriptions_line_2[3] = { "Same behavior as the HTML5 port.", "enter huts from other level layouts.", ""};
std::string option_main_treasure_huts_descriptions_line_3[3] = { "", "Same behavior as the original game.", ""};
std::string option_main_archer_freq_choices[6] = { "Original", "New", "Low", "Medium", "High", "Non-stop" };
std::string option_main_archer_freq_descriptions_line_1[6] = { "Archers appear more in later levels.", "Archers appear more in later levels.", "Archers do not appear often.", "Archers appear semi-often.", "Archers appear very often.", "Archers appear non-stop!"};
std::string option_main_archer_freq_descriptions_line_2[6] = { "Same behavior as the original game.", "New scaling that's more evenly", "", "", "", ""};
std::string option_main_archer_freq_descriptions_line_3[6] = { "", "distributed between levels.", "", "", "", ""};
std::string option_main_knight_behavior_choices[2] = { "Original", "New" };
std::string option_main_knight_behavior_descriptions_line_1[2] = { "Knights briefly pause and change", "Knights do not change their" };
std::string option_main_knight_behavior_descriptions_line_2[2] = { "position after you die.", "movement after you die." };
std::string option_main_knight_behavior_descriptions_line_3[2] = { "", "Behavior is similar to the HTML5 port." };
std::string option_main_frame_rate_choices[8] = { "16 FPS [original]", "30 FPS", "32 FPS [2x]", "48 FPS [3x]", "60 FPS", "90 FPS", "120 FPS", "144 FPS [9x]" };
std::string option_main_frame_rate_descriptions_line_1[1] = { "Higher frame rate = smoother gameplay." };
std::string option_main_frame_rate_descriptions_line_2[1] = { "(Original Flash game is 16 FPS)" };
std::string option_main_frame_rate_descriptions_line_3[1] = { "" };
std::string option_main_level_tran_choices[2] = { "Original", "Button Press" };
std::string option_main_level_tran_descriptions_line_1[2] = { "After beating a level, the next", "After beating a level, press " + INPUT_CONFIRM };
std::string option_main_level_tran_descriptions_line_2[2] = { "level will load automatically.", "to load the next level." };
std::string option_main_level_tran_descriptions_line_3[2] = { "", "" };
std::string option_main_comment_freq_choices[7] = { "Off", "Very Low", "Low", "Normal", "High", "Very High", "Let's-Player" };
std::string option_main_comment_freq_descriptions_line_1[7] = { "Strong Bad does not talk.", "Strong Bad rarely talks.", "Strong Bad talks less than usual.", "Strong Bad talks as often", "Strong Bad talks a bit more often.", "Strong Bad talks much more often.", "Strong Bad won't shut up!" };
std::string option_main_comment_freq_descriptions_line_2[7] = { "", "", "", "as he did in the original game.", "", "", "(He talks twice as much as usual.)"};
std::string option_main_comment_freq_descriptions_line_3[7] = { "", "", "", "", "", "", "" };
#if defined(WII_U) || defined(VITA) || defined(SWITCH) || defined(ANDROID) || defined(PSP) || defined(WII) || defined(GAMECUBE) || defined(THREEDS) || defined(XBOX)
std::string press_confirm_to_apply = "(Press " + INPUT_CONFIRM + " to apply, may take a few seconds)";
#else
std::string press_confirm_to_apply = "(Press " + INPUT_CONFIRM + " to apply)";
#endif
#if defined(THREEDS)
std::string option_main_scaling_choices[4] = { "N/A", "N/A", "N/A", "N/A" };
std::string option_main_scaling_descriptions_line_1[4] = { "(Unused)", "(Unused)", "(Unused)", "(Unused)" };
std::string option_main_scaling_descriptions_line_2[4] = { "", "", "", "" };
std::string option_main_scaling_descriptions_line_3[4] = { "", "", "", "" };
#elif defined(PSP)
std::string option_main_scaling_choices[4] = { "Pixel-Perfect", "", "Full", "Full Game" };
std::string option_main_scaling_descriptions_line_1[4] = { "Scale so that everything uses integer", "", "Scale everything so that", "Scale the game to fill the screen." };
std::string option_main_scaling_descriptions_line_2[4] = { "scaling. Accurate, but small.", "", "the overlay fills the screen.", "The overlay will be cut off." };
std::string option_main_scaling_descriptions_line_3[4] = { press_confirm_to_apply, "", press_confirm_to_apply, press_confirm_to_apply };
#elif defined(WII) || defined(GAMECUBE) || defined(XBOX)
std::string option_main_scaling_choices[4] = { "Pixel-Perfect", "", "", "Full Game" };
std::string option_main_scaling_descriptions_line_1[4] = { "Scale so that everything uses integer", "", "", "Scale the game to fill the screen." };
std::string option_main_scaling_descriptions_line_2[4] = { "scaling. The most accurate.", "", "", "The overlay will be cut off." };
std::string option_main_scaling_descriptions_line_3[4] = { press_confirm_to_apply, "", "", press_confirm_to_apply };
#elif defined(WII_U)
std::string option_main_scaling_choices[4] = { "Pixel-Perfect", "Pixel-Perfect Game", "", "" };
std::string option_main_scaling_descriptions_line_1[4] = { "Scale so that everything uses integer", "Scale the game to be as big as possible", "", "" };
std::string option_main_scaling_descriptions_line_2[4] = { "scaling. Accurate, but small.", "with integer scaling. The most accurate.", "", "" };
std::string option_main_scaling_descriptions_line_3[4] = { press_confirm_to_apply, press_confirm_to_apply, "", "" };
#elif defined(SWITCH)
std::string option_main_scaling_choices[4] = { "Pixel-Perfect", "Pixel-Perfect Game", "Full", "" };
std::string option_main_scaling_descriptions_line_1[4] = { "Scale so that everything uses integer", "Scale the game to be as big as possible", "Scale everything so that", "" };
std::string option_main_scaling_descriptions_line_2[4] = { "scaling. Accurate, but possibly small.", "with integer scaling. The most accurate.", "the overlay fills the screen.", "" };
std::string option_main_scaling_descriptions_line_3[4] = { press_confirm_to_apply, press_confirm_to_apply, press_confirm_to_apply, "" };
#else
std::string option_main_scaling_choices[4] = { "Pixel-Perfect", "Pixel-Perfect Game", "Full", "Full Game" };
std::string option_main_scaling_descriptions_line_1[4] = { "Scale so that everything uses integer", "Scale the game to be as big as possible", "Scale everything so that", "Scale the game to fill the screen." };
std::string option_main_scaling_descriptions_line_2[4] = { "scaling. Accurate, but possibly small.", "with integer scaling. The most accurate.", "the overlay fills the screen.", "The overlay will be cut off." };
std::string option_main_scaling_descriptions_line_3[4] = { press_confirm_to_apply, press_confirm_to_apply, press_confirm_to_apply, press_confirm_to_apply };
#endif
std::string option_main_difficulty_descriptions_line_1[1] = { "Change settings related to" };
std::string option_main_difficulty_descriptions_line_2[1] = { "game difficulty." };
std::string option_main_cosmetic_descriptions_line_1[1] = { "Change settings related to" };
std::string option_main_cosmetic_descriptions_line_2[1] = { "game audio and video." };
std::string option_main_cheats_descriptions_line_1[1] = { "Toggle secret cheats." };
std::string option_main_cheats_descriptions_line_2[1] = { "Follow the hints!" };
std::string option_main_credits_descriptions_line_1[1] = { "View the credits." };
std::string option_main_reset_settings_descriptions_line_1[1] = { "Reset all settings to default." };
std::string option_main_quit_descriptions_line_1[1] = { "Quit the game." };
std::string option_cheats_inf_lives_descriptions_line_1[1] = { "Infinite lives, play forever!" };
std::string option_cheats_inf_lives_descriptions_line_2[1] = { "Unlocking this cheat also unlocks" };
std::string option_cheats_inf_lives_descriptions_line_3[1] = { "more starting lives options." };
std::string option_cheats_debug_mode_descriptions_line_1[1] = { "Enable Debug Mode." };
std::string option_cheats_debug_mode_descriptions_line_2[1] = { "Press buttons to do things." };
std::string option_cheats_big_head_mode_descriptions_line_1[1] = { "Give Trogdor a more husky head." };
std::string option_cheats_noclip_descriptions_line_1[1] = { "Remove the game's difficulty" };
std::string option_cheats_noclip_descriptions_line_2[1] = { "by walking through cottages." };
#if defined(WII_U) || defined(VITA) || defined(PSP) || defined(WII) || defined(GAMECUBE) || defined(THREEDS) || defined(XBOX)
#if defined(WII_U)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING A WII U?!";
#elif defined(VITA)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING A VITA?!";
#elif defined(PSP)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING A PSP?!";
#elif defined(WII)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING A WII?!";
#elif defined(GAMECUBE)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING A GAMECUBE?!";
#elif defined(THREEDS)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING A 3DS?!";
#elif defined(XBOX)
std::string console_quit_line_1 = "WHY ARE YOU STILL USING AN XBOX?!";
#else
std::string console_quit_line_1 = "WHY ARE YOU STILL USING WINDOWS 98?!";
#endif
std::string console_quit_line_2 = "WHAT IS WRONG WITH YOU?! STOP BUYING";
std::string console_quit_line_3 = "CONSOLES FROM THRIFT STORES, MAN!";
#else
#if defined(SWITCH)
std::string console_quit_line_1 = "Switch over? Virus equals very yes?";
#elif defined(ANDROID)
std::string console_quit_line_1 = "Android over? Virus equals very yes?";
#else
std::string console_quit_line_1 = "Computer over? Virus equals very yes?";
#endif
std::string console_quit_line_2 = "";
std::string console_quit_line_3 = "";
#endif
std::string option_quit_descriptions_line_1[7] = { "Hey Player, I really like your", "Strong Bad's gonna get the", "Are you sure you want to quit this", "Don't leave yet -- There's a", "If you leave now, how will you", "Click \"Keep playing\" to email strong bad", console_quit_line_1};
std::string option_quit_descriptions_line_2[7] = { "\"quitting to play a different game\"", "high score, man, 50,000.", "great game?", "peasant around that corner!", "get ye flask?", "", console_quit_line_2};
std::string option_quit_descriptions_line_3[7] = { "costume.", "", "", "", "", "", console_quit_line_3};

void InitializeMenus() {
	TTF_Init();
	// if the menu font would end up being the same as font_serif_white_6_mult, just use that instead of initializing an exclusive font
	if (font_serif_white_6_mult.size == getDesiredFontSize(&font_serif_white_8)) {
		menuFont = &font_serif_white_6_mult;
		setFont(menuFont, "fonts/serif_v01.ttf", 8, 5, TTF_STYLE_NORMAL, color_white, true);
	} else {
		menuFont = &font_serif_white_8;
		setFont(menuFont, "fonts/serif_v01.ttf", 8, 5, TTF_STYLE_NORMAL, color_white, true);
	}

	/* Options Menu */
	menu_main.prepareMenu(MENU_NUM_OPTIONS, 6, &sprite_menu_cursor, false, 1, 32 + (16 * (screenScale_menu >= 2)), 168 + (8 * (screenScale_menu >= 2)), 0, 25, 175, 25, 15, 0, 0, true);
	if (!menusAreInitialized) {
		for (i = 0; i < MENU_NUM_OPTIONS; i++) {
			menu_main.options[i] = new MenuOption();
		}
	}

	MENU_DIFFICULTY->prepareMenuOption("Difficulty Settings", option_empty,
		option_main_difficulty_descriptions_line_1, option_main_difficulty_descriptions_line_2, option_empty,
		"", 1, true, 0, true);
	MENU_COSMETIC->prepareMenuOption("Cosmetic Settings", option_empty,
		option_main_cosmetic_descriptions_line_1, option_main_cosmetic_descriptions_line_2, option_empty,
		"", 1, true, 0, true);
	MENU_CHEATS->prepareMenuOption("Cheats", option_empty,
		option_main_cheats_descriptions_line_1, option_main_cheats_descriptions_line_2, option_empty,
		"", 1, true, 0, true);
	MENU_RESET_SETTINGS->prepareMenuOption("Reset to Default", option_empty,
		option_main_reset_settings_descriptions_line_1, option_empty, option_empty,
		"", 1, true, 0, true);
	MENU_CREDITS->prepareMenuOption("Credits", option_empty,
		option_main_credits_descriptions_line_1, option_empty, option_empty,
		"", 1, true, 0, true);
	MENU_QUIT->prepareMenuOption("Quit Game", option_empty,
		option_main_quit_descriptions_line_1, option_empty, option_empty,
		"", 1, true, 0, true);

	/* Difficulty Settings Menu */
	menu_difficulty.prepareMenu(DIFFICULTY_NUM_OPTIONS, 6, &sprite_menu_cursor, false, 1, 32 + (16 * (screenScale_menu >= 2)), 160 + (16 * (screenScale_menu >= 2)), 0, 25, 175, 25, 15, 0, 0, true);
	if (!menusAreInitialized) {
		for (i = 0; i < DIFFICULTY_NUM_OPTIONS; i++) {
			menu_difficulty.options[i] = new MenuOption();
		}
	}
	MENU_STARTING_LEVEL->prepareMenuOption("Starting Level", option_main_starting_level_choices,
		option_main_starting_level_descriptions_line_1, option_empty, option_empty,
		"", 10, true, 0, true);
	MENU_EXTRA_LIVES->prepareMenuOption("Extra Lives", option_main_extra_lives_choices,
		option_main_extra_lives_descriptions_line_1, option_empty, option_empty,
		"", 9, true, 3, true);
	MENU_LIVES_INTERVAL->prepareMenuOption("Lives Interval", option_main_lives_interval_choices,
		option_main_lives_interval_descriptions_line_1, option_main_lives_interval_descriptions_line_2, option_main_lives_interval_descriptions_line_3,
		"", 8, false, 0, true);
	MENU_PEASANT_PENALTY->prepareMenuOption("Peasant Penalty", option_on_off,
		option_main_peasant_penalty_descriptions_line_1, option_main_peasant_penalty_descriptions_line_2, option_main_peasant_penalty_descriptions_line_3,
		"", 2, false, 0, true);
	MENU_TREASURE_HUTS->prepareMenuOption("Treasure Huts", option_main_treasure_huts_choices,
		option_main_treasure_huts_descriptions_line_1, option_main_treasure_huts_descriptions_line_2, option_main_treasure_huts_descriptions_line_3,
		"", 3, false, 0, true);
	MENU_ARCHER_FREQ->prepareMenuOption("Archer Frequency", option_main_archer_freq_choices,
		option_main_archer_freq_descriptions_line_1, option_main_archer_freq_descriptions_line_2, option_main_archer_freq_descriptions_line_3,
		"", 6, false, 0, true);
	MENU_KNIGHT_BEHAVIOR->prepareMenuOption("Knight Behavior", option_main_knight_behavior_choices,
		option_main_knight_behavior_descriptions_line_1, option_main_knight_behavior_descriptions_line_2, option_main_knight_behavior_descriptions_line_3,
		"", 2, false, 0, true);

	/* Cosmetic Settings Menu */
	menu_cosmetic.prepareMenu(COSMETIC_NUM_OPTIONS, 6, &sprite_menu_cursor, false, 1, 32 + (16 * (screenScale_menu >= 2)), 160 + (16 * (screenScale_menu >= 2)), 0, 25, 175, 25, 15, 0, 0, true);
	if (!menusAreInitialized) {
		for (i = 0; i < COSMETIC_NUM_OPTIONS; i++) {
			menu_cosmetic.options[i] = new MenuOption();
		}
	}
	MENU_FRAME_RATE->prepareMenuOption("Frame Rate", option_main_frame_rate_choices,
		option_main_frame_rate_descriptions_line_1, option_main_frame_rate_descriptions_line_2, option_main_frame_rate_descriptions_line_3,
		"", 8, true, 0, true);
	MENU_LEVEL_TRAN->prepareMenuOption("Level Transition", option_main_level_tran_choices,
		option_main_level_tran_descriptions_line_1, option_main_level_tran_descriptions_line_2, option_main_level_tran_descriptions_line_3,
		"", 2, false, 0, true);
	MENU_MUSIC->prepareMenuOption("Music", option_main_music_choices,
		option_main_music_descriptions_line_1, option_main_music_descriptions_line_2, option_main_music_descriptions_line_3,
		"", 2, false, 0, true);
	MENU_COMMENT_FREQ->prepareMenuOption("Commentary", option_main_comment_freq_choices,
		option_main_comment_freq_descriptions_line_1, option_main_comment_freq_descriptions_line_2, option_main_comment_freq_descriptions_line_3,
		"", 7, false, 3, true);
	MENU_SCALING->prepareMenuOption("Screen Scaling", option_main_scaling_choices,
		option_main_scaling_descriptions_line_1, option_main_scaling_descriptions_line_2, option_main_scaling_descriptions_line_3,
		"", 4, false, scalingType, true);

	/* Cheats Menu */
	menu_cheats.prepareMenu(CHEAT_NUM_OPTIONS, 6, &sprite_menu_cursor, false, 1, 32 + (16 * (screenScale_menu >= 2)), 160 + (16 * (screenScale_menu >= 2)), 0, 25, 175, 25, 15, 0, 0, true);
	if (!menusAreInitialized) {
		for (i = 0; i < CHEAT_NUM_OPTIONS; i++) {
			menu_cheats.options[i] = new MenuOption();
		}
	}
	CHEAT_INF_LIVES->prepareMenuOption("Infinite Lives", option_on_off,
		option_cheats_inf_lives_descriptions_line_1, option_cheats_inf_lives_descriptions_line_2, option_cheats_inf_lives_descriptions_line_3,
		"Secret Code?!?!", 2, true, 1, true);
	CHEAT_BIG_HEAD_MODE->prepareMenuOption("Big Head Mode", option_on_off,
		option_cheats_big_head_mode_descriptions_line_1, option_empty, option_empty,
		"S&K Mushroom Pulley", 2, true, 1, true);
	CHEAT_NOCLIP->prepareMenuOption("Noclip", option_on_off,
		option_cheats_noclip_descriptions_line_1, option_cheats_noclip_descriptions_line_2, option_empty,
		"1994 Country", 2, true, 1, true);
	CHEAT_DEBUG_MODE->prepareMenuOption("Debug Mode", option_on_off,
		option_cheats_debug_mode_descriptions_line_1, option_cheats_debug_mode_descriptions_line_2, option_empty,
		"Class of 1981", 2, true, 1, true);

	/* Credits Notebook */
	menu_credits.prepareMenuNotebook(5, 304, 216, 2);
	if (!menusAreInitialized) {
		for (i = 0; i < 5; i++) {
			menu_credits.pages[i] = new MenuPage();
		}
	}
	menu_credits.pages[0]->prepareMenuPage(9, 0, 30, 20, 1);
	menu_credits.pages[0]->setTextLine(0, "- ORIGINAL FLASH GAME -");
	menu_credits.pages[0]->setTextLine(1, "");
	menu_credits.pages[0]->setTextLine(2, "Programming: Jonathan Howe");
	menu_credits.pages[0]->setTextLine(3, "Design: Mike Chapman, Matt Chapman");
	menu_credits.pages[0]->setTextLine(4, "");
	menu_credits.pages[0]->setTextLine(5, "https://homestarrunner.com");
	menu_credits.pages[0]->setTextLine(6, "");
	menu_credits.pages[0]->setTextLine(7, "https://www.youtube.com/user");
	menu_credits.pages[0]->setTextLine(8, "/homestarrunnerdotcom");

	menu_credits.pages[1]->prepareMenuPage(8, 0, 30, 20, 1);
	menu_credits.pages[1]->setTextLine(0, "- THIS RECREATION -");
	menu_credits.pages[1]->setTextLine(1, "");
	menu_credits.pages[1]->setTextLine(2, "Mips96");
	menu_credits.pages[1]->setTextLine(3, "");
	menu_credits.pages[1]->setTextLine(4, "https://github.com/Mips96");
	menu_credits.pages[1]->setTextLine(5, "/Trogdor-Reburninated");
	menu_credits.pages[1]->setTextLine(6, "");
	menu_credits.pages[1]->setTextLine(7, "v1.2");

	menu_credits.pages[2]->prepareMenuPage(5, 0, 30, 20, 1);
	menu_credits.pages[2]->setTextLine(0, "- STINKOMAN MUSIC -");
	menu_credits.pages[2]->setTextLine(1, "");
	menu_credits.pages[2]->setTextLine(2, "Jonathan Howe");
	menu_credits.pages[2]->setTextLine(3, "");
	menu_credits.pages[2]->setTextLine(4, "U.Z. Inu");

	menu_credits.pages[3]->prepareMenuPage(7, 0, 30, 20, 1);
	menu_credits.pages[3]->setTextLine(0, "- LIBRARIES -");
	menu_credits.pages[3]->setTextLine(1, "");
#if defined(SDL1)
	menu_credits.pages[3]->setTextLine(2, "SDL");
	menu_credits.pages[3]->setTextLine(3, "SDL_image");
	menu_credits.pages[3]->setTextLine(4, "SDL_mixer");
	menu_credits.pages[3]->setTextLine(5, "SDL_ttf");
	menu_credits.pages[3]->setTextLine(6, "SDL_gfx");
#else
	menu_credits.pages[3]->setTextLine(2, "SDL2");
	menu_credits.pages[3]->setTextLine(3, "SDL2_image");
#if defined(PSP)
	menu_credits.pages[3]->setTextLine(4, "OSLibAudio");
#else
	menu_credits.pages[3]->setTextLine(4, "SDL2_mixer");
#endif
	menu_credits.pages[3]->setTextLine(5, "SDL2_ttf");
	menu_credits.pages[3]->setTextLine(6, "");
#endif

	menu_credits.pages[4]->prepareMenuPage(9, 0, 30, 20, 1);
	menu_credits.pages[4]->setTextLine(0, "- WANT MORE? -");
	menu_credits.pages[4]->setTextLine(1, "");
	menu_credits.pages[4]->setTextLine(2, "Trogdor: Reburninated is available");
	menu_credits.pages[4]->setTextLine(3, "for a wide variety of");
	menu_credits.pages[4]->setTextLine(4, "homebrew-enabled systems, old and new.");
	menu_credits.pages[4]->setTextLine(5, "Play it everywhere!");
	menu_credits.pages[4]->setTextLine(6, "");
	menu_credits.pages[4]->setTextLine(7, "https://github.com/Mips96");
	menu_credits.pages[4]->setTextLine(8, "/Trogdor-Reburninated");

	/* Quit Menu */
	menu_quit.prepareMenu(QUIT_NUM_OPTIONS, 7, &sprite_menu_cursor, false, 1, 128 + (16 * (screenScale_menu >= 2)), 160 + (16 * (screenScale_menu >= 2)), 0, 132, 60, 25, 15, 0, 0, true);
	if (!menusAreInitialized) {
		for (i = 0; i < QUIT_NUM_OPTIONS; i++) {
			menu_quit.options[i] = new MenuOption();
		}
	}
	QUIT_BACK->prepareMenuOption("Keep playing", option_empty,
		option_quit_descriptions_line_1, option_quit_descriptions_line_2, option_quit_descriptions_line_3,
		"", 1, false, 0, true);
	QUIT_CONFIRM->prepareMenuOption("Quit it!", option_empty,
		option_quit_descriptions_line_1, option_quit_descriptions_line_2, option_quit_descriptions_line_3,
		"", 1, false, 0, true);

	/* Initializing Values */
	if (!menusAreInitialized) {
		MENU_EXTRA_LIVES->choiceIsAllowed[6] = false;
		MENU_EXTRA_LIVES->choiceIsAllowed[7] = false;
		MENU_EXTRA_LIVES->choiceIsAllowed[8] = false;
		CHEAT_INF_LIVES->setLocked(true);
		CHEAT_DEBUG_MODE->setLocked(true);
		CHEAT_BIG_HEAD_MODE->setLocked(true);
		CHEAT_NOCLIP->setLocked(true);
#if defined(THREEDS) || defined(PSP) || defined(WII) || defined(GAMECUBE) || defined(XBOX)
		MENU_SCALING->choiceIsAllowed[1] = false;
#endif
#if defined(THREEDS) || defined(WII) || defined(GAMECUBE) || defined(XBOX) || defined(WII_U)
		MENU_SCALING->choiceIsAllowed[2] = false;
#endif
#if defined(THREEDS) || defined(WII_U) || defined(SWITCH)
		MENU_SCALING->choiceIsAllowed[3] = false;
#endif
	}

	menu_main.updateOptionPositions();
	menu_difficulty.updateOptionPositions();
	menu_cosmetic.updateOptionPositions();
	menu_cheats.updateOptionPositions();
	menu_quit.updateOptionPositions();

	menusAreInitialized = true;

	TTF_CloseFont(menuFont->font);
	TTF_Quit();
}

State_Settings getSettings() {
	return {
		DEFAULT_VOLUME_MUSIC,
		100,
		MENU_STARTING_LEVEL->index,
		MENU_EXTRA_LIVES->index,
		MENU_LIVES_INTERVAL->index,
		MENU_PEASANT_PENALTY->index,
		MENU_TREASURE_HUTS->index,
		MENU_ARCHER_FREQ->index,
		MENU_KNIGHT_BEHAVIOR->index,
		MENU_FRAME_RATE->index,
		MENU_LEVEL_TRAN->index,
		MENU_MUSIC->index,
		MENU_COMMENT_FREQ->index,
		MENU_SCALING->index,
		CHEAT_INF_LIVES->index,
		CHEAT_DEBUG_MODE->index,
		CHEAT_BIG_HEAD_MODE->index,
		CHEAT_NOCLIP->index,
		CHEAT_INF_LIVES->optionIsLocked,
		CHEAT_DEBUG_MODE->optionIsLocked,
		CHEAT_BIG_HEAD_MODE->optionIsLocked,
		CHEAT_NOCLIP->optionIsLocked
	};
}

void updateFrameRate() {
	switch (MENU_FRAME_RATE->index) {
		case 1:
			enhancedFrameRate = 30;
			break;
		case 2:
			enhancedFrameRate = 32;
			break;
		case 3:
			enhancedFrameRate = 48;
			break;
		case 4:
			enhancedFrameRate = 60;
			break;
		case 5:
			enhancedFrameRate = 90;
			break;
		case 6:
			enhancedFrameRate = 120;
			break;
		case 7:
			enhancedFrameRate = 144;
			break;
		default:
			enhancedFrameRate = 16;
			break;
	}
	frameRateMult = static_cast<double>(ORIGINAL_FRAME_RATE) / enhancedFrameRate;
	popRandVal = enhancedFrameRate * 100 / ORIGINAL_FRAME_RATE;
	frameRate = enhancedFrameRate;
	ticksPerFrame = (Uint32)(1000 / frameRate);
}