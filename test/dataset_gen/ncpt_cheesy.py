import os
import random

cheesy_str = (
    "Really_Cheesy_Flavour_Corn_Puffs._Ingredients:_Corn_(Maize),_Rapeseed_Oil,"
    "_Cheese_Flavour_[Whey_Perrmeate(Contains_MILK),_Dried_Cheese_(From_MILK)"
    "_(6%),)_Salt,_Cheese_Solids_(From_MILK),_Dairy_Solids_(From_MILK),_Yeast_Extract,"
    "_Potassium_Cholride,__Acid_(Lactic_acid),_Flavour_Enhancer_(Disodium_5'-rionucleotides),"
    "_Colours_(Paprika_Extract,_Anatto_Norbixin),_Natural_Flavouring_(Contains_MILK)]."
)

target_size = 1024 # e.g., 500 KB
chunk_size = len(cheesy_str)
random_block_size = 32    # 32 bytes of random data for variety

with open("1KB-cheesy.bin", "wb") as f:
    bytes_written = 0
    while bytes_written < target_size:
        # Write the cheesy string
        f.write(cheesy_str.encode('utf-8'))
        bytes_written += chunk_size

        # Write a random block
        rand_block = os.urandom(random_block_size)
        f.write(rand_block)
        bytes_written += random_block_size