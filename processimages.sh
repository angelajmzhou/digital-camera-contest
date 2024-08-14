
# Folder containing the files
folder=~/"round I"

# Suffix of the files to use as input
suffix=".SRF.clear"

# Find all files with the specified suffix
files=$(find "$folder" -type f -name "*$suffix")

# Check if any files are found
if [ -z "$files" ]; then
    echo "No files with suffix $suffix found in $folder"
    exit 1
fi

# Run the executable with the found files as arguments
./image $files

