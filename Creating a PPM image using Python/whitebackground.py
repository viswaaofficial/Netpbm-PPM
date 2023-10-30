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


# Example usage:
if __name__ == "__main__":
    image_width = 200
    image_height = 100
    background_color = (255, 255, 255)  # White

    pixels_data = generate_pixels(image_width, image_height, background_color)
    write_image("output_image.ppm", pixels_data)
