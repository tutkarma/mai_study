def hex_chunk(data, order):
    return int.from_bytes(data, byteorder=order)


with open("out.data", "rb") as f:
    w = hex_chunk(f.read(4), "little") 
    h = hex_chunk(f.read(4), "little")
    print(f"w:{w} h:{h}")

    length = h * w
    for k in range(1, length + 1):
        if k % w == 0:
            END_SYM = "\n"
        else:
            END_SYM = " "
        chunk = hex_chunk(f.read(4), "big")
        print(f"{chunk:08X}", end=END_SYM)

