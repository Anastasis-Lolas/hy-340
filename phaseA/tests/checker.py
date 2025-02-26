import subprocess
import sys

# Check if the correct number of arguments is provided
if len(sys.argv) != 4:
    print("Usage: python checker.py <executable> <input_file> <expected_output_file>")
    sys.exit(1)

# Get arguments from the command line
executable = sys.argv[1]
input_file = sys.argv[2]
expected_output_file = sys.argv[3]

# Run the executable and capture the output
with open(input_file, "r") as infile:
    result = subprocess.run(executable, stdin=infile, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

# Read the expected output
with open(expected_output_file, "r") as outfile:
    expected_output = outfile.readlines()

# Read the actual output line by line
actual_output = result.stdout.splitlines()

# Function to remove all spaces (ignores differences in whitespace)
def normalize(line):
    return "".join(line.split()) 
    
# Normalize both outputs before comparing
normalized_actual = [normalize(line) for line in actual_output]
normalized_expected = [normalize(line) for line in expected_output]

# Compare outputs ignoring whitespace
if normalized_actual == normalized_expected:
    print("✅ Output is correct!")
else:
    print("❌ Output is incorrect! Differences (ignoring spaces):")

if len(normalized_actual) != len(normalized_expected):
    print("❌ Output is incorrect! Differences (ignoring spaces):")
    sys.exit(1)

for i, (actual_line, expected_line) in enumerate(zip(normalized_actual, normalized_expected)):
    if actual_line != expected_line:
        print(f"Line {i + 1}:")
        print(f"  Expected: {expected_output[i].strip() if i < len(expected_output) else '<missing>'}")
        print(f"  Actual:   {actual_output[i].strip() if i < len(actual_output) else '<missing>'}")
        print()

