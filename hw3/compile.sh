#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <path-to-c-file>"
  exit 1
fi

c_file=$1
output_file="${c_file%.c}.out"

while true; do
  new_checksum=$(md5sum "$c_file" | awk '{print $1}')

  if [ "$new_checksum" != "$old_checksum" ]; then
    clear  # Clear the terminal
    gcc -o "$output_file" "$c_file"  # Replace with your compile command
    old_checksum=$new_checksum
  fi

  sleep 1  # Add a delay to reduce CPU usage
done

