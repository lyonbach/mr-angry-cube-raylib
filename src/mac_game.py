from mac_types import *

from mac_helpers import Heading, get_v3
from mac_camera import FollowCamera
from mac_mr_angry_cube import MrAngryCube


class KeyEventsListener:
    def __init__(self, player: MrAngryCube):
        self._player = player

    def update(self):
        if raylib.is_key_pressed(raylib.KEY_W):
            self._player.behaviour.next_heading = Heading.EAST
        elif raylib.is_key_pressed(raylib.KEY_S):
            self._player.behaviour.next_heading = Heading.WEST
        elif raylib.is_key_pressed(raylib.KEY_D):
            self._player.behaviour.next_heading = Heading.NORTH
        elif raylib.is_key_pressed(raylib.KEY_A):
            self._player.behaviour.next_heading = Heading.SOUTH
        elif raylib.is_key_pressed(raylib.KEY_Q):
            self._player.behaviour.next_heading = Heading.NO_HEADING
        elif raylib.is_key_pressed(raylib.KEY_SPACE):
            self._player.behaviour._can_move = True


class Game:
    def __init__(self):
        self.screen_width  = 800
        self.screen_height = 800
        self.target_fps = 60

    def main(self):
        raylib.init_window(self.screen_width, self.screen_height, "MrAngryCube")
        raylib.set_target_fps(self.target_fps)  # Set our game to run at 60 frames-per-second

        mr_angry_cube = MrAngryCube(get_v3(y=1.0))
        camera = FollowCamera(mr_angry_cube, get_v3(10, 10, 10))
        key_events_listener = KeyEventsListener(mr_angry_cube)

        # Main game loop
        while not raylib.window_should_close():  # Detect window close button or ESC key
            # player.behaviour.heading = Heading.NO_HEADING
            # Update
            raylib.begin_drawing()
            raylib.clear_background(raylib.DARKBLUE)

            # Draw
            raylib.begin_mode_3d(camera.get())
            raylib.draw_grid(20, 1.0)

            key_events_listener.update()

            mr_angry_cube.update()
            mr_angry_cube.draw()

            camera.update()

            raylib.end_mode_3d()

            raylib.end_drawing()
        # De-Initialization
        raylib.close_window()  # Close window and OpenGL context
