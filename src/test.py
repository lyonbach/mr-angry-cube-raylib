import time
import pyray as raylib


def vector2_round(v: raylib.Vector2) -> raylib.Vector2:
    return raylib.Vector2(round(v.x), round(v.y))

def vector2_print(vec: raylib.Vector2) -> str:
    print(f"({vec.x}, {vec.y})")

def vector2_abs(vec: raylib.Vector2) -> raylib.Vector2:
    return raylib.Vector2(abs(vec.x), abs(vec.y))

def get_target(position: raylib.Vector2) -> raylib.Vector2:
    signs = raylib.Vector2(int(position.x >= 0), int(position.y >= 0))
    rounded = vector2_round(vector2_abs(position))
    return raylib.vector2_multiply(rounded, signs)

dt = .005
heading = raylib.Vector2(1.0, 0.0)
velocity = raylib.Vector2(1.0, 0.0)
position = raylib.Vector2(0.16, 0.0)

target = get_target(position)
 
vector2_print(position)
vector2_print(target)
