print('hello,world')
print('hello','python')
print("waht\'s your name?")
# 这是第一个python程序的注释
print('打印一个浮点:')
a = 6.6e6
print(a)

print("打印字符串:")
a = "穿着长衫"+","+"站着喝酒"+'。'
print(a)
print(r'他说"要一碟茴香豆"')
print(r'''
今天天气不错啊！
你看起来像一条"小鱼"
谢谢！^-^''')
print("2017 + 0x11 = ",2017 + 0x11)

print('\nlist的使用:')
friend = [1,'wzy',2,'xm',3,'xh']
print(friend)
print (friend[0],friend[1])
print (friend[-6],friend[-5])
friend.append(4)#默认在尾部插入
friend.append("xx")
print(friend)
friend.insert(0,"cloud")#添加索引插入
friend.insert(0, 0)
print(friend)
#frined.pop(2)
friend.pop()#尾部删除
friend.pop()
print(friend)
print('\ntuple的使用:')
#元组一旦创建，就不能修改
t = ('xh','xw')
print(t)
print(t[0])
t = (66,)#定义单元素的tuple
print(t)
print(t[0])
t = ('xh','xw',[66,33])
print(t)
t[2][0] = 33#元组一旦创建，指向不能修改
t[2][1] = 66
print(t)
