#pragma once
#include "raylib.h"
#include <string>


constexpr float CAMERA_CHASE_VECTOR_INCREMENT = .5f;
constexpr float CAMERA_VERTICAL_TOP_DOWN = 8.0f;
constexpr float CAMERA_VERTICAL_NORMAL = 4.0f;
constexpr float CAMERA_VERTICAL_CHASE = 2.0f;
constexpr float MAX_CAMERA_CHASE_DISTANCE = 20.0f;
constexpr float MIN_CAMERA_CHASE_DISTANCE = 2.0f;
constexpr float CAMERA_TARGET_UPDATE_SPEED_COEFF = 2.4f;
constexpr float CAMERA_UPDATE_SPEED_COEFF = 4 * CAMERA_TARGET_UPDATE_SPEED_COEFF;
constexpr float STANDARD_CAMERA_SHAKE_STRENGTH = 1.0f;
constexpr float FACE_HI_CAMERA_SHAKE_STRENGTH = 2 * STANDARD_CAMERA_SHAKE_STRENGTH;

constexpr unsigned int MINIMUM_ANGER = 0;
constexpr unsigned int MAXIMUM_ANGER = 10;

constexpr Vector3 DEFAULT_CURRENT_ROTATION_AXIS = { 0.0, 0.0f, 0.0f };
constexpr Vector3 DEFAULT_NEXT_ROTATION_AXIS = { 0.0, 0.0f, 0.0f };

const static std::string NEW_GAME_BUTTON_TEXT = "New Game";
const static std::string LOAD_LEVEL_BUTTON_TEXT = "Load Level";
const static std::string EXIT_GAME_BUTTON_TEXT = "Exit Game";
const static std::string CONTINUE_BUTTON_TEXT = "Continue";
const static std::string RETURN_TO_MAIN_MENU_BUTTON_TEXT = "Return To Main Menu";
const static std::string SELECT_LEVEL_BUTTON_TEXT = "Select";

const static std::string MODEL_EXTENSION = ".obj";
const static std::string TEXTURE_EXTENSION = ".png";
const static std::string VERTEX_SHADER_EXTENSION = ".vs";
const static std::string FRAGMENT_SHADER_EXTENSION = ".fs";
const static std::string ASSETS_DIRECTORY = "assets";
const static std::string MODELS_DIRECTORY = "models";
const static std::string TEXTURES_DIRECTORY = "textures";
const static std::string SHADERS_DIRECTORY = "shaders";

const static std::string M_MR_ANGRY_CUBE_FACE = "mr-angry-cube-face";
const static std::string M_MR_ANGRY_CUBE_BODY = "mr-angry-cube-body";

const static std::string T_MR_ANGRY_CUBE_FACE_NORMAL = "mr-angry-cube-face-normal";
const static std::string T_MR_ANGRY_CUBE_FACE_ANGER_LEVEL_1 = "mr-angry-cube-face-anger-level-1";
const static std::string T_MR_ANGRY_CUBE_FACE_ANGER_LEVEL_2 = "mr-angry-cube-face-anger-level-2";
const static std::string T_MR_ANGRY_CUBE_FACE_ANGER_LEVEL_3 = "mr-angry-cube-face-anger-level-3";
const static std::string T_MR_ANGRY_CUBE_FACE_ANGER_LEVEL_4 = "mr-angry-cube-face-anger-level-4";
const static std::string T_MR_ANGRY_CUBE_FACE_ANGER_LEVEL_5 = "mr-angry-cube-face-anger-level-5";
const static std::string T_MR_ANGRY_CUBE_BODY_NORMAL = "mr-angry-cube-body-normal";
