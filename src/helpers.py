import pyray as raylib

def v3_quantize(vector: raylib.Vector3) -> raylib.Vector3:
    return raylib.Vector3(round(vector.x), round(vector.y), round(vector.z))

def v3_print(vector: raylib.Vector3):
    print(f"Vector3(x={vector.x}, y={vector.y}, z={vector.z})")
