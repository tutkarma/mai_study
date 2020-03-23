EPS = 0.001
C = 0.01 - 0.1j
Z_0 = 0.0 + 0.35j


def main():
    z_cur, z_next = Z_0, 0.0 + 0.0j
    i = 1
    print(f'C = {C}')
    print(f'Z_0 = {Z_0}')
    while abs(z_cur - z_next) > EPS:
        print(f'Iteration #{i}')
        z_next, z_cur = z_cur * z_cur + C, z_next
        print(f'Z_{i} = {z_next}')
        i += 1

if __name__ == '__main__':
    main()