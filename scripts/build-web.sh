echo "[1]   -> Building for WEB -";
echo "[1][1]-> Adjusting environment...";

PROJECT_ROOT=$(realpath $(pwd)/..)

# Check if readme.md exists.
if [ -f "$PROJECT_ROOT/readme.md" ]; then
    echo "Set project root succesfully."
else
    echo "Could not locate readme.md. This script should be in the scripts directory under the project root! Leaving...";
    exit 1;
fi

TARGET_PATH=$PROJECT_ROOT/build-web

if [ -z "$EMSCRIPTEN_PATH" ]; then
    echo "Error: EMSCRIPTEN_PATH is not set. Please set it before running this script.";
    exit 1;
fi
source $EMSCRIPTEN_PATH/emsdk_env.sh;

if [ -z "$RAYLIB_PATH" ]; then
    echo "Error: RAYLIB_PATH is not set. Please set it before running this script.";
    exit 1;
fi

if [ ! -d "$RAYLIB_PATH" ]; then
    echo "Error: RAYLIB_PATH does not point to a valid directory. Please check the path.";
    exit 1;
fi

# Test emsdk command
em++ --version
if [ $? -eq 0 ]; then
    echo "[1][1]-> emsdk command executed successfully.";
else
    echo "[1][1]-> emsdk command failed.";
    exit;
fi

# Collect cpp files
echo "[2]   -> Building...";
echo "[2][1]-> Collecting source files...";
CPP_FILES=$(find $PROJECT_ROOT/src -type f -name "*.cpp");
CPP_FILES_ARG="";
echo "[2][2]-> Found those files:"
for cpp_file in $CPP_FILES;
do
    echo $cpp_file;
    CPP_FILES_ARG="$CPP_FILES_ARG $cpp_file";
done

echo "[2][3]->Copying files..."
# mkdir -p
mkdir -p $TARGET_PATH/assets
for folder in "models" "shaders" "textures";
do
    mkdir -p $TARGET_PATH/assets/$folder
    for file in $(ls $PROJECT_ROOT/$folder);
    do 
        echo Copying $file...;
        cp $PROJECT_ROOT/$folder/$file $TARGET_PATH/assets/$folder;
        if [ $? -eq 1 ]; then
            echo "Could not copy file: $file...";
            exit 1;
        fi
    done
done

echo $CPP_FILES_ARG;
echo "[2][4]-> Succesfully generated the arguments.";

cd $PROJECT_ROOT/build-web

em++ $CPP_FILES \
    -o $TARGET_PATH/index.html \
    -I$RAYLIB_PATH/include \
    -I$PROJECT_ROOT/src \
    -L$RAYLIB_PATH \
    -lraylib \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    -s FULL_ES2=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s FORCE_FILESYSTEM=1 \
    -std=c++20 \
    -DPLATFORM_WEB \
    --preload-file assets/models \
    --preload-file assets/shaders \
    --preload-file assets/textures

if [ $? -eq 0 ]; then
    echo "[2][5]-> Build was succesful!";
else
    echo "[2][5]-> Build failed!";
    exit;
fi
