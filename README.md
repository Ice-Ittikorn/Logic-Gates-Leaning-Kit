docker build -t esp-idf-kit .
docker run -it --rm -v $(pwd):/workspace esp-idf-kit
idf.py build

source ~/store/Logic/v5.5.2/esp-idf/export.sh