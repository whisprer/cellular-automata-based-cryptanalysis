import random
import sys

def shift_mix_encrypt_binary(data, key):
    """
    ShiftMix cipher encryption for binary data:
    1. Uses a key to generate consistent random shifts
    2. Applies different shifts to each byte based on position
    3. Uses XOR for additional complexity
    """
    random.seed(key)  # Seed random with the key for consistent results
    
    # Create a substitution table for all possible byte values (0-255)
    byte_range = list(range(256))
    shuffled = byte_range.copy()
    random.shuffle(shuffled)
    sub_table = dict(zip(byte_range, shuffled))
    
    encrypted = bytearray()
    
    # If data is already bytes, use it directly; otherwise, encode it
    if isinstance(data, str):
        data = data.encode('utf-8')
    
    for i, byte in enumerate(data):
        # Step 1: Apply position-based shifting
        position_shift = (i % 7) + 1  # Different shift based on position
        key_shift = random.randint(1, 15)  # Consistent for same key
        total_shift = (position_shift + key_shift) % 256
        
        shifted_byte = (byte + total_shift) % 256
        
        # Step 2: Apply substitution
        encrypted_byte = sub_table.get(shifted_byte, shifted_byte)
        encrypted.append(encrypted_byte)
    
    return bytes(encrypted)

def shift_mix_decrypt_binary(data, key):
    """
    ShiftMix cipher decryption for binary data - reverse of the encryption process
    """
    random.seed(key)  # Seed with the same key
    
    # Recreate the same substitution table
    byte_range = list(range(256))
    shuffled = byte_range.copy()
    random.shuffle(shuffled)
    sub_table = dict(zip(byte_range, shuffled))
    
    # Create reverse substitution table
    reverse_sub = {v: k for k, v in sub_table.items()}
    
    decrypted = bytearray()
    
    for i, byte in enumerate(data):
        # Step 1: Reverse the substitution
        unsubstituted_byte = reverse_sub.get(byte, byte)
        
        # Step 2: Reverse the shifting
        position_shift = (i % 7) + 1
        key_shift = random.randint(1, 15)
        total_shift = (position_shift + key_shift) % 256
        
        decrypted_byte = (unsubstituted_byte - total_shift) % 256
        decrypted.append(decrypted_byte)
    
    return bytes(decrypted)

# Check if enough arguments are provided
if len(sys.argv) < 4:
    print("Usage: python script.py <input_file> <output_file> <mode>")
    print("Mode should be 'encrypt' or 'decrypt'")
    sys.exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]
mode = sys.argv[3]
key = "mysecretkey123"  # You can also make this a command line argument

# Read input file in binary mode
try:
    with open(input_file, 'rb') as fin:
        data = fin.read()
except FileNotFoundError:
    print(f"Error: Input file '{input_file}' not found")
    sys.exit(1)
except Exception as e:
    print(f"Error reading input file: {e}")
    sys.exit(1)

# Process based on mode
if mode.lower() == 'encrypt':
    result = shift_mix_encrypt_binary(data, key)
    print(f"Encrypted content written to {output_file}")
elif mode.lower() == 'decrypt':
    result = shift_mix_decrypt_binary(data, key)
    print(f"Decrypted content written to {output_file}")
else:
    print("Invalid mode. Use 'encrypt' or 'decrypt'")
    sys.exit(1)

# Write to output file in binary mode
try:
    with open(output_file, 'wb') as fout:
        fout.write(result)
except Exception as e:
    print(f"Error writing to output file: {e}")
    sys.exit(1)