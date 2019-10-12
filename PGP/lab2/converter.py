import argparse

from PIL import Image


def bin_to_image(src, dest):
    with open(src, 'rb') as f:
        arr = f.read()

    w = int.from_bytes(arr[:4], byteorder='little')
    h = int.from_bytes(arr[4:8], byteorder='little')

    img = Image.frombytes('RGBA', (w, h), arr[8:], 'raw')
    img.save(dest, 'PNG')


def image_to_bin(src, dest):
    res = b''

    with Image.open(src) as img:
        w, h = map(lambda x: x.to_bytes(4, byteorder='little'), img.size)
        res += w + h

        for pixel in img.convert('RGBA').getdata():
            r, g, b, alpha = map(lambda x: x.to_bytes(1, byteorder='little'), pixel)
            res += r + g + b + alpha

    with open(dest, 'wb') as f:
        f.write(res)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--encode', action='store_true', help='Convert image to bin')
    parser.add_argument('--decode', action='store_true', help='Convert bin to image')
    parser.add_argument('--src', required=True)
    parser.add_argument('--dest', required=True)
    args = parser.parse_args()
    
    if args.encode and args.decode:
        print('Choose encode or decode')
    elif args.decode:
        bin_to_image(args.src, args.dest)
    elif args.encode:
        image_to_bin(args.src, args.dest)
