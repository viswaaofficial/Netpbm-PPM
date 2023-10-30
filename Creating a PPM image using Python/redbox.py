from typing import List, Tuple

# Define a type alias for pixel data
Color = Tuple[int, int, int]
Pixels = List[List[Color]]


def write_image(file_name: str, pixels: Pixels):
    height = len(pixels)
    width = len(pixels[0])

    with open(file_name, "w") as f:
        f.write(f"P3 {width} {height} 255\n")

        for i in range(height):
            for j in range(width):
                f.write(f"{pixels[i][j][0]} {pixels[i][j][1]} {pixels[i][j][2]} ")


def generate_pixels(width, height, bg_color: Color) -> Pixels:
    result: Pixels = []
    for _ in range(height):
        row: List[Color] = []
        for _ in range(width):
            row.append(bg_color)
        result.append(row)
    return result


def draw_rect(pixels: Pixels, x, y, w, h, color: Color):
    for i in range(h):
        for j in range(w):
            pixels[y + i][x + j] = color


def main():
    image = generate_pixels(800, 600, (255, 255, 255))
    draw_rect(image, 100, 100, 200, 200, (255, 0, 0))
    write_image("generated.ppm", image)


if __name__ == "__main__":
    main()
