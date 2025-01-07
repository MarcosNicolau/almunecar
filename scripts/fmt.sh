#!/bin/bash

FORMATTER="clang-format"
ROOT_DIR="libs"
EXIT_CODE=0

if [ "$1" == "--fix" ]; then
    echo "Auto-fixing formatting issues..."
    for file in $(find "$ROOT_DIR" -type f \( -name '*.c' -o -name '*.h' \)); do
        echo "Formatting $file..."
        $FORMATTER -i "$file" 
        if [ $? -ne 0 ]; then
            echo "Error formatting $file"
            EXIT_CODE=1
        fi
    done
    echo "Formatting fixed."
    exit 0
fi

for file in $(find "$ROOT_DIR" -type f \( -name '*.c' -o -name '*.h' \)); do
    DIFF=$($FORMATTER --output-replacements-xml "$file" | grep -c "<replacement ")
    if [ "$DIFF" -ne 0 ]; then
        echo "Formatting error in $file. See the suggested changes below:"
        $FORMATTER -style=file "$file" | diff -u "$file" -
        EXIT_CODE=1
    fi
done

exit $EXIT_CODE
