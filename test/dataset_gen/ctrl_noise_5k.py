import os

def generate_random_noise(filename: str, size_in_bytes: int):
    # Generate cryptographically secure random bytes
    random_data = os.urandom(size_in_bytes)

    # Write the random bytes to a binary file
    with open(filename, 'wb') as file:
        file.write(random_data)

if __name__ == '__main__':
    output_filename = 'random_noise.bin'
    noise_size = 5 * 1024  # 5KB = 5120 bytes

    generate_random_noise(output_filename, noise_size)

    print(f'Successfully created {output_filename} with {noise_size} bytes of random data.')
