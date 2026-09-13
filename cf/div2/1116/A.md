## Idea

Sort the three nonnegative numbers so that

$$
0 \le a \le b \le c.
$$

The minimum possible range is

$$
\boxed{\min(c-a,\ b)}.
$$

With zero operations, the range is $c-a$. Replacing the largest number with the sum of the other two gives range $b$. After the first operation, no later operation can decrease the range.

## Proof

### Zero operations

The range of the sorted triple $(a,b,c)$ is simply

$$
c-a.
$$

### One operation

If we replace the largest number $c$ with $a+b$, the numbers become

$$
a,\ b,\ a+b.
$$

Since the numbers are nonnegative, $a \le b \le a+b$. The new range is

$$
(a+b)-a=b.
$$

If we replace $a$ with $b+c$, the sorted triple becomes $(b,c,b+c)$. Its range is

$$
(b+c)-b=c.
$$

If we replace $b$ with $a+c$, the sorted triple becomes $(a,c,a+c)$. Its range is

$$
(a+c)-a=c.
$$

Since $b \le c$, the best first operation replaces the largest value and gives range $b$.

### Further operations cannot improve the range

After any operation, one number is the sum of the other two. Thus, after sorting, the triple has the form

$$
x,\ y,\ x+y, \qquad 0 \le x \le y \le x+y.
$$

Its range is $(x+y)-x=y$. Consider every possible next operation.

**Replace the largest value $x+y$.** The sum of the other two numbers is already $x+y$, so nothing changes. The range stays $y$.

**Replace $y$.** Its new value is $x+(x+y)=2x+y$. The sorted triple becomes $(x,x+y,2x+y)$, with range

$$
(2x+y)-x=x+y \ge y.
$$

**Replace $x$.** Its new value is $y+(x+y)=x+2y$. The sorted triple becomes $(y,x+y,x+2y)$, with range

$$
(x+2y)-y=x+y \ge y.
$$

Every possible next operation leaves the range unchanged or increases it. The resulting triple again has one value equal to the sum of the other two, so the same argument applies after every later operation.

Therefore, **once the first operation is performed, no later operation can decrease the range**.

Only two choices need to be considered: zero operations, giving $c-a$, or the best single operation, giving $b$. Hence,

$$
\boxed{\text{Minimum range} = \min(c-a,\ b)}.
$$

## Complexity

Sorting three values and computing the answer take $O(1)$ time and $O(1)$ extra space per test case.
