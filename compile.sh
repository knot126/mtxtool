echo "Building mtxtool..."
clang++ -o mtxtool -Wall -Wextra -O2 main.cpp jpeg.cpp zlib.cpp mtx.cpp png.cpp -lz -lpng -ljpeg
echo "Building mtx2png..."
clang++ -o mtx2png -Wall -Wextra -O2 main.cpp jpeg.cpp zlib.cpp mtx.cpp png.cpp -lz -lpng -ljpeg -DCONVERT_PNG_ONLY
