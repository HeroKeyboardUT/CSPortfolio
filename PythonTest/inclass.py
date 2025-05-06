def ngto2(x):
    return x>=2 and not [ i for i in range(2,x) if x % i==0 ]

def pythagons(n):
    return ([a,b,c] for a in range(1,n) for b in range(a,n) for c in range(b,n) if a**2 + b**2 == c**2)

def first_letters(words):
    return [word[0] for word in words]

def factorial(n):
    return 1 if n == 0 else n * factorial(n - 1)

def pascal_row(n):
    return [factorial(n) // (factorial(k) * factorial(n - k)) for k in range(n + 1)]

def take(lst):
    return [lst[i] for i in range(0, len(lst), 2)]

def skip(lst):
    return [lst[i] for i in range(1, len(lst), 2)]

def sort_array(source_array):
    return [x if x % 2 == 0 else next(iter(sorted(y for y in source_array if y % 2 != 0))) for x in source_array]
    



def multiply_all(arr, factor):
    # TODO : using lamba function
    return list(map(lambda x: x * factor, arr))


def exponentiate(base):
    # TODO
    def power(exponent):
        return base ** exponent
    return power

foo = exponentiate(2)
print(foo(3))  # 8

# print(multiply_all([1, 2, 3, 4], 2))
# print(sort_array([5, 3, 2, 8, 1, 4]))
