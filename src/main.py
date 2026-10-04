import json
from pathlib import Path
from mac_game import Game


if __name__ == "__main__":

    settings_file  = Path("settings.json")
    if not settings_file.exists():
        raise FileNotFoundError("Settings file not found!")  # TODO LOAD DEFAULT SETTINGS
    with open(str(settings_file), 'r') as stream:
        settings = json.load(stream)
    game = Game(settings)  # TODO INITIALIZATION
    game.main()

print("Game exited.")
