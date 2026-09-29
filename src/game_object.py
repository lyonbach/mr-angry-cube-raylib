import pyray as raylib

class IGameObject:
    def __init__(self):
        self.position: raylib.Vector3 = raylib.Vector3(0, 0, 0)
        self.velocity: raylib.Vector3 = raylib.Vector3(0, 0, 0)

    def draw(self):
        raise NotImplementedError("The draw method must be implemented by subclasses.")

    def update(self):
        raise NotImplementedError("The update method must be implemented by subclasses.")
