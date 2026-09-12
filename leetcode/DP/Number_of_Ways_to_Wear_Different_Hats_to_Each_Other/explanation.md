# LeetCode 1434: Number of Ways to Wear Different Hats to Each Other

[Problem statement](https://leetcode.com/problems/number-of-ways-to-wear-different-hats-to-each-other/)

## Problem

Each person has a list of hats they like. Give every person exactly one hat from their list. No two people can use the same hat number. Count the valid assignments modulo `1,000,000,007`.

There are at most 10 people and 40 hat numbers.

## Why process hats?

If we process people and track used hats with a bitmask, there can be `2^40` subsets of hats. This is too large.

Instead, process hats in order and track which people already have a hat. With at most 10 people, there are only `2^10 = 1024` masks.

For each hat, either leave it unused or give it to one eligible person. Then move to the next hat. This prevents a hat from being assigned twice.

## Reverse the input

The input maps each person to the hats they like. Build `H_To_P` to map each hat to the people who like it:

```cpp
H_To_P[hat-1].push_back(i);
```

The code uses zero-based hat indices. Index `0` represents hat number `1`, and index `39` represents hat number `40`.

## Bitmask

Bit `p` is 1 if person `p` already has a hat. Otherwise, it is 0. The rightmost bit represents person 0.

| Mask | People who have hats |
|---|---|
| `000` | Nobody |
| `001` | Person 0 |
| `101` | Persons 0 and 2 |
| `111` | Everybody |

Check whether a person has a hat:

```cpp
peopleMask & (1LL << people)
```

Mark that person as assigned:

```cpp
int newMask = peopleMask | (1LL << people);
```

The mask for all `m` people is `(1LL << m) - 1`.

## Recursive state

`dfs(currHat, peopleMask)` counts the ways to finish assigning hats using indices `currHat` through `39`, given the people already assigned in `peopleMask`.

The identities of their earlier hats do not affect future choices. All earlier hat indices have already been processed.

### Base case

```cpp
if(currHat==40){
    return peopleMask==((1LL<<m)-1);
}
```

At index 40, all hats have been processed. Return 1 if everyone has a hat, because this is one complete assignment. Otherwise, return 0.

### Give the current hat to one person

For each person in `H_To_P[currHat]`, skip them if they already have a hat. Otherwise, set their bit and add:

```cpp
dfs(currHat + 1, newMask)
```

Each call moves to the next hat immediately, so only one person receives the current hat in that branch.

### Leave the current hat unused

Keep the mask unchanged and add:

```cpp
dfs(currHat + 1, peopleMask)
```

Add this branch once using assignment:

```cpp
ways = (ways + dfs(currHat + 1, peopleMask)) % MOD;
```

Using `ways += (ways + ...) % MOD` would add the existing count twice.

## Memoization

Different earlier assignments can reach the same `(currHat, peopleMask)`. They have exactly the same remaining choices, so store the answer in `memo[currHat][peopleMask]`.

The initial value `-1` means the state has not been calculated. A reference lets the code read and update the memo cell directly:

```cpp
int& result = memo[currHat][peopleMask];
```

Start with `dfs(0, 0)`: no hat has been processed and no person has a hat.

## Example

```text
Person 0 likes [1, 2]
Person 1 likes [2, 3]
```

Using actual hat numbers, the successful branches are:

```text
Skip hat 1
  Give hat 2 to person 0
    Give hat 3 to person 1 -> 1 assignment

Give hat 1 to person 0
  Give hat 2 to person 1 -> 1 assignment, skipping all later hats
  Skip hat 2
    Give hat 3 to person 1 -> 1 assignment
```

All other branches leave someone without a hat. The answer is 3.

## Correctness proof

Consider any state `(currHat, peopleMask)`.

If all hats have been processed, there is exactly one completed assignment when every person has a hat, and none otherwise. This is the base case.

For a remaining hat, every valid completion either leaves that hat unused or gives it to exactly one person who likes it and has no hat yet. The recursion considers every such choice. These choices are disjoint because they specify different uses of the current hat.

After each choice, the recursion moves to the next hat with the correct mask. Assuming these smaller remaining problems are counted correctly, adding their counts gives the correct answer for the current state. By induction on the number of remaining hats, every state is correct.

Thus, `dfs(0, 0)` counts every valid full assignment exactly once. Taking remainders during addition gives that count modulo `MOD`.

## Complexity

There are at most `40 * 2^m` memoized states. Each state checks at most `m` people.

- Time: `O(40 * m * 2^m)`.
- Space: `O(40 * 2^m + 40 * m)` for the memo and reverse mapping. The recursion depth is at most 41.

The submitted implementation is in [code.cpp](code.cpp).
