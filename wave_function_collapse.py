import pyray as rl
import itertools

COLORS = {
 'a': rl.RED,
 'b': rl.GREEN,
 'c': rl.BLUE,
 'd': rl.PURPLE,
 'e': rl.YELLOW,
 'x': rl.GRAY
}


class IPiece:
    def __init__(self, row: int, col: int, size: int, map_offset_x: int, map_offset_y: int):
        self.north = 'x'
        self.south = 'x'
        self.west  = 'x'
        self.east  = 'x'
        self.r: int = row
        self.c: int = col
        self.size: int = size

        self._map_offset_x = map_offset_x
        self._map_offset_y = map_offset_y
        self._offset = 2

    @property
    def x(self):
        return self.r * self.size

    @property
    def y(self):
        return self.c * self.size

    def draw(self):
        side_thickness = self.size // 5
        side_length = self.size - 2 * side_thickness
        # north rectangle
        rl.draw_rectangle(
            self.x + side_thickness + self._map_offset_x,
            self.y + self._map_offset_y,
            side_length,
            self.size // 5,
            COLORS.get(self.north, rl.RED)
        )

        # east rectangle
        rl.draw_rectangle(
            self.x + self.size - side_thickness + self._map_offset_x,
            self.y + side_thickness + self._map_offset_y,
            self.size // 5,
            side_length,
            COLORS.get(self.east, rl.RED)
        )

        # # south rectangle
        rl.draw_rectangle(
            self.x + side_thickness + self._map_offset_x,
            self.y + self.size - side_thickness + self._map_offset_y,
            side_length,
            side_thickness,
            COLORS.get(self.south, rl.RED)
        )

        # west rectangle
        rl.draw_rectangle(
            self.x + self._map_offset_x,
            self.y + side_thickness + self._map_offset_y,
            side_thickness,
            side_length,
            COLORS.get(self.west, rl.RED)
        )

class Piece(IPiece):
    def __init__(self, row: int, col: int, size: int, map_offset_x: int, map_offset_y: int):
        super().__init__(row, col, size, map_offset_x, map_offset_y)
        self.north = 'x'
        self.east  = 'x'
        self.south = 'x'
        self.west  = 'x'

class Map:
    def __init__(self, size: tuple, piece_length: int):
        self.rows, self.cols = size
        self.size = size
        self.piece_length = piece_length
        self.pieces: list[list[IPiece]] = []
        for j in range(self.cols):
            for i in range(self.rows):
                self.pieces.append(Piece(i, j, piece_length, 300, 250))

    def generate(self):
        import random
        available = [k for k in COLORS.keys() if k != 'x']

        variants = (
            [f'xx{p[0]}{p[1]}' for p in itertools.product(available, repeat=2)] +
            [f'{p[0]}x{p[1]}{p[2]}' for p in itertools.product(available, repeat=3)] +
            [f'x{p[0]}{p[1]}x' for p in itertools.product(available, repeat=2)] +
            [f'{p[0]}{p[1]}x{p[2]}' for p in itertools.product(available, repeat=3)]
        )

        for piece in self.pieces:
            v = random.choice(variants)
            piece.north = v[0]
            piece.east  = v[1]
            piece.south = v[2]
            piece.west  = v[3]


def main():
    width, height = 1200, 800

    rl.init_window(width, height, "raylib [core] example - basic window")
    rl.set_target_fps(60)
    game_map = Map((10, 5), 60)
    game_map.generate()

    camera = rl.Camera2D()

    while not rl.window_should_close():
        rl.begin_drawing()
        rl.clear_background(rl.BLACK)
        for piece in game_map.pieces:
            rl.trace_log(rl.TraceLogLevel.LOG_INFO, "test")
            piece.draw()

        rl.end_drawing()

    rl.close_window()

if __name__ == "__main__":
    main()
