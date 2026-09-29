import time
import pyray as raylib

def v3_print(vector: raylib.Vector3):
    print(f"Vector3(x={vector.x}, y={vector.y}, z={vector.z})")

def v3_quantize(vector: raylib.Vector3) -> raylib.Vector3:
    return raylib.Vector3(round(vector.x), round(vector.y), round(vector.z))

dt: float = .002
position : raylib.Vector3 = raylib.Vector3(1.0, 0.0, 0.0)
velocity : raylib.Vector3 = raylib.Vector3(1.0, 0.0, 0.0)
heading  : raylib.Vector3 = raylib.Vector3(0.0, 0.0, 0.0)
vector_dt: raylib.Vector3 = raylib.Vector3(dt , dt , dt )


while True:
    position = raylib.vector3_add(position, raylib.vector3_multiply(velocity, vector_dt))
    v3_print(position)
    v3_print(v3_quantize(position))

    time.sleep(1)
