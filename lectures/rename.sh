#!/bin/zsh

# Iterate over all files starting with 'w' in the current directory
for file in w*; do
  # Extract the number following 'w' in the filename
  num=$(echo $file | sed -n 's/^w0000\([0-9]\+\).*/\1/p')

  # Check if the number is a single digit
  if [[ $num -lt 10 ]]; then
    # Construct the new filename with leading zero
    newfile=$(echo $file | sed "s/w0000$num/w$num/")
    
    # Rename the file
    mv "$file" "$newfile"
  fi
done

