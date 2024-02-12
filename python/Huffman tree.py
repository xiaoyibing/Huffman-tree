import turtle


class Node:  # 用于储存数据
    def __init__(self, data, weight):
        self.data = data  # 名称
        self.weight = weight  # 权值
        self.left = None  # 左孩子
        self.right = None


def build_huffman_tree(nodes):
    while len(nodes) > 1:
        nodes.sort(key=lambda x: x.weight)  # 根据权值排序
        left = nodes.pop(0)  # 删除并且返回第一个值
        right = nodes.pop(0)  # 删除并且返回（原来的）第二个值
        new_node = Node(None, left.weight + right.weight)
        new_node.left = left
        new_node.right = right
        nodes.append(new_node)  # 添加新的节点（图像中蓝色的节点）
    return nodes[0]  # 返回总节点（最大的节点）


def draw_circle(x, y, number, color):
    turtle.pensize(1)  # 画笔粗细
    turtle.color('black', color)
    turtle.penup()
    turtle.goto(x, y)  # 圆的正上方坐标
    turtle.pendown()
    turtle.begin_fill()  # 涂色
    turtle.circle(-20)  # 顺时针画圆
    turtle.end_fill()
    turtle.penup()
    turtle.goto(x + 25, y - 35)  # 在屏幕上输出权值
    turtle.write(number, font=("宋体", 15, 'normal'))


def draw_line(x1, y1, x2, y2):  # 链接
    turtle.pensize(3)
    turtle.penup()
    turtle.goto(x1, y1)  # 父节点的圆的下方
    turtle.pendown()
    turtle.goto(x2, y2)  # 叶节点的圆的上方


def display_huffman_tree(root, x, y, a, b):  # 递归输出（a,b表示子节点向下或向外移动多少距离）
    if root:
        if root.data is None:
            draw_circle(x, y, root.weight, 'blue')
        else:
            draw_circle(x, y, root.weight, 'yellow')
            turtle.goto(x, y - 30)
            turtle.write(root.data, font=("宋体", 15, 'normal'))  # 输出名称
        if root.left:  # 如果存在
            draw_line(x, y - 40, x - a, y - b)
            display_huffman_tree(root.left, x - a, y - b, a / 2, b - 30)  # 递归输出
        if root.right:
            draw_line(x, y - 40, x + a, y - b)
            display_huffman_tree(root.right, x + a, y - b, a / 2, b - 30)


def main():
    n=int(input("请输入个数："))
    nodes = []  # 保存节点
    print("请输入数据名称和数据的权值，以逗号分隔(如：a,1,e,3,r,4,t,5)(注意用英文逗号):")
    data = input().split(',')  # 分离数据名称和权值

    for i in range(n):  # 依次加入类中
        nodes.append(Node(data[i * 2], int(data[i * 2 + 1])))
    root = build_huffman_tree(nodes)  # 构建关系

    turtle.speed(0)  # 画笔速度
    turtle.hideturtle()

    display_huffman_tree(root, 0, 300, 200, 200)  # 输出图案

    turtle.done()


main()
