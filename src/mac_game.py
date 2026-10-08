from typing import Dict, List

from mac_types import *
from mac_helpers import Heading, get_v3
from mac_camera import FollowCamera
from mac_mr_angry_cube import MrAngryCube
from mac_enemy import EnemyBase
from mac_game_object import GameObject


class KeyEventsListener:
    def __init__(self, player: MrAngryCube):
        self._player = player

    def update(self):
        if raylib.is_key_pressed(raylib.KEY_W):
            self._player.behaviour.next_heading = Heading.NORTH
        elif raylib.is_key_pressed(raylib.KEY_S):
            self._player.behaviour.next_heading = Heading.SOUTH
        elif raylib.is_key_pressed(raylib.KEY_D):
            self._player.behaviour.next_heading = Heading.EAST
        elif raylib.is_key_pressed(raylib.KEY_A):
            self._player.behaviour.next_heading = Heading.WEST
        elif raylib.is_key_pressed(raylib.KEY_Q):
            self._player.behaviour.next_heading = Heading.NO_HEADING
        elif raylib.is_key_pressed(raylib.KEY_SPACE):
            self._player.behaviour._can_move = True


class Game:
    _instance = None
    def __new__(cls, settings: Dict=None):
        if cls._instance is None:
            cls._instance = super(Game, cls).__new__(cls)
            cls._instance._init_game(settings)
        return cls._instance

    def _init_game(self, settings: Dict=None):
        self.game_objects: List[GameObject] = []
        self.settings: Dict = {}
        if settings:
            self.settings.update(settings)
            self.screen_width  = settings["window"]["width"]
            self.screen_height = settings["window"]["height"]
            self.target_fps    = settings["window"]["fps"]

    def main(self):
        print(self.settings["window"]["full_screen"])
        raylib.init_window(self.screen_width, self.screen_height, "MrAngryCube")
        raylib.set_target_fps(self.target_fps)
        if self.settings["window"]["full_screen"]:
            raylib.toggle_fullscreen()

        # TODO THIS SHOULD BE LOADED FROM THE LEVEL DATA
        player_position = self.settings["player"]["start_position"]
        mr_angry_cube = MrAngryCube("player", get_v3(*player_position))
        enemy = EnemyBase("enemy", get_v3(x=3), 1)

        self.game_objects.extend([mr_angry_cube, enemy])
        
        camera_offset = self.settings["camera"]["offset"]
        camera_fovy = self.settings["camera"]["fovy"]
        follow_camera = FollowCamera(mr_angry_cube, get_v3(*camera_offset), camera_fovy)

        key_events_listener = KeyEventsListener(mr_angry_cube)



        # Main game loop
        while not raylib.window_should_close():  # Detect window close button or ESC key
            # Update
            raylib.begin_drawing()
            raylib.clear_background(raylib.DARKBLUE)

            # Draw
            raylib.begin_mode_3d(follow_camera.get())
            raylib.draw_grid(20, 1.0)

            key_events_listener.update()
            for object in self.game_objects:
                object.update()
                object.draw()

            follow_camera.update()

            raylib.end_mode_3d()
            raylib.end_drawing()

        # De-Initialization
        raylib.close_window()  # Close window and OpenGL context
