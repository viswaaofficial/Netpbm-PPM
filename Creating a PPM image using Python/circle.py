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


def fill_circle(pixels: Pixels, x, y, r, color: Color):
    for i in range(y - r, y + r):
        for j in range(x - r, x + r):
            if (j - x) ** 2 + (i - y) ** 2 <= r * r:
                pixels[i][j] = color


def main():
    image = generate_pixels(800, 600, (255, 255, 255))
    fill_circle(image, 400, 300, 150, (200, 50, 50))
    write_image("circle.ppm", image)


if __name__ == "__main__":
    main()
