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

const std::string NEW_GAME_BUTTON_TEXT = "New Game";
const std::string LOAD_LEVEL_BUTTON_TEXT = "Load Level";
const std::string EXIT_GAME_BUTTON_TEXT = "Exit Game";
const std::string CONTINUE_BUTTON_TEXT = "Continue";
const std::string RETURN_TO_MAIN_MENU_BUTTON_TEXT = "Return To Main Menu";
const std::string SELECT_LEVEL_BUTTON_TEXT = "Select";

const std::string MODEL_EXTENSION = ".obj";
const std::string TEXTURE_EXTENSION = ".png";
const std::string VERTEX_SHADER_EXTENSION = ".vs";
const std::string FRAGMENT_SHADER_EXTENSION = ".fs";
const std::string ASSETS_DIRECTORY = "assets";
const std::string MODELS_DIRECTORY = "models";
const std::string TEXTURES_DIRECTORY = "textures";
const std::string SHADERS_DIRECTORY = "shaders";
