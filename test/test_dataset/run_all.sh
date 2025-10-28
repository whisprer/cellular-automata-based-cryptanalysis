#!/bin/bash

# run_all.sh - Process all .bin files with CACA
# Usage: 
#   chmod +x run_all.sh
#   ./run_all.sh

# Color codes for pretty output (optional, comment out if not wanted)
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if caca executable exists
if [ ! -f "./caca" ]; then
    echo -e "${RED}Error: caca executable not found in current directory${NC}"
    echo "Please compile first with:"
    echo "  g++ -std=c++17 -O3 -march=native -o caca caca_main.cpp -lm -pthread"
    exit 1
fi

# Count .bin files
total_files=$(ls -1 *.bin 2>/dev/null | wc -l)

if [ "$total_files" -eq 0 ]; then
    echo -e "${YELLOW}No .bin files found in current directory${NC}"
    exit 0
fi

echo -e "${GREEN}Found $total_files .bin files to process${NC}"
echo "======================================="

# Process each .bin file
file_count=0
for file in *.bin; do
    file_count=$((file_count + 1))
    echo -e "${BLUE}[$file_count/$total_files] Processing: $file${NC}"
    
    # Run CACA with autodetected thread count
    ./caca_fast -f "$file" -v
    
    echo "---------------------------------------"
    echo
done

echo -e "${GREEN}Processing complete! Analyzed $file_count files.${NC}"

# Optional: Create a summary if output files were generated
    if ls caca_results_*.bin >/dev/null 2>&1; then
        echo -e "${YELLOW}Output files generated:${NC}"
        ls -lh caca_results_*.bin
    
    fi
done

echo "Done processing all .bin files"