import sys

class FlawedPRNG:
    """
    A deliberately flawed PRNG that produces predictable sequences
    with low entropy and strong patterns.
    """
    def __init__(self, seed):
        self.state = 0
        # Simple seed initialization - very predictable
        for char in seed:
            self.state = (self.state * 31 + ord(char)) & 0xFFFFFFFF
        
        # Introduce a small initial sequence to make it look random
        # but actually quite predictable
        for _ in range(10):
            self.next_byte()
    
    def next_byte(self):
        """Generate the next byte in the sequence"""
        # Linear congruential generator with poor parameters
        # This will create repeating patterns
        self.state = (self.state * 1664525 + 1013904223) & 0xFFFFFFFF
        
        # Only use lower 8 bits, severely limiting entropy
        return self.state & 0xFF
    
    def get_bytes(self, n):
        """Get n bytes from the generator"""
        return bytes(self.next_byte() for _ in range(n))


def encrypt_file(input_file, output_file, key):
    """
    Encrypt a file using the flawed PRNG
    """
    prng = FlawedPRNG(key)
    
    try:
        with open(input_file, 'rb') as fin:
            data = fin.read()
            
        # Simple XOR encryption with the flawed PRNG stream
        # This will create patterns in the output that correlate with patterns in the input
        encrypted = bytearray()
        key_stream = prng.get_bytes(len(data))
        
        for i in range(len(data)):
            encrypted.append(data[i] ^ key_stream[i])
        
        with open(output_file, 'wb') as fout:
            fout.write(encrypted)
            
        print(f"Encrypted {len(data)} bytes from {input_file} to {output_file}")
        
    except Exception as e:
        print(f"Error: {e}")
        return False
        
    return True


def decrypt_file(input_file, output_file, key):
    """
    Decrypt a file using the flawed PRNG
    (Note: With XOR, encryption and decryption are the same operation)
    """
    return encrypt_file(input_file, output_file, key)


if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python flawed_prng.py [encrypt/decrypt] input_file output_file [key]")
        sys.exit(1)
        
    mode = sys.argv[1].lower()
    input_file = sys.argv[2]
    output_file = sys.argv[3]
    key = sys.argv[4] if len(sys.argv) > 4 else "default_key"
    
    if mode == "encrypt":
        encrypt_file(input_file, output_file, key)
    elif mode == "decrypt":
        decrypt_file(input_file, output_file, key)
    else:
        print("Invalid mode. Use 'encrypt' or 'decrypt'")