def calculate_weights(input_file):
    # 打开原始文本文件并读取内容
    with open(input_file, 'r', encoding='ANSI') as f:
        text = f.read()

    # 统计每个字符的出现次数
    weights = {}
    for char in text:
        if char in weights:  # 如果曾经统计过
            weights[char] += 1
        else:  # 第一次统计
            weights[char] = 1

    return weights


def write_weights(output_file, weights):
    # 将字符及其对应的权重写入到另一个文本文件中
    with open(output_file, 'w', encoding='ANSI') as f:
        for char, weight in weights.items():
            if char == '\n':  # 换行符额外处理，在c语言中“\\n”是两个字符“\”+“n”的意思
                f.write(f"\\n {weight}\n")
            else:
                f.write(f"{char} {weight}\n")


def main():
    input_file = "yuan_wen.txt"  # 原始文本文件
    output_file = "quan_zhi.txt"  # 权重文本文件

    # 计算字符权重
    weights = calculate_weights(input_file)
    # 将权重写入文件
    write_weights(output_file, weights)


main()
