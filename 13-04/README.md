# directory_filter

Lists directory entries whose filenames match a regex pattern, with metadata.

## Usage

```
./directory_filter [path] pattern
```

## Comparison with grep

| | `directory_filter` | `ls -la \| grep` |
|---|---|---|
| Input | directory entries | text lines |
| Match target | filename only | entire `ls` output line |
| Metadata | type, permissions, size, timestamp | raw `ls` format |
| Regex | `std::regex` (ECMAScript) | POSIX BRE / ERE (`-E`) |
| Recursion | no (single level) | no (grep alone) |

```bash
# equivalent commands
./directory_filter . '\.cpp$'
ls | grep '\.cpp$'

# grep can also match on metadata (date, size, permissions) — directory_filter cannot
ls -la | grep '^d'

# grep with -r searches file contents, not filenames
grep -r 'pattern' .
```

`std::regex` uses ECMAScript syntax by default, so quantifiers, groups, and anchors
work the same as in JavaScript. POSIX `grep` uses BRE by default; pass `-E` for ERE
(closer to ECMAScript, but still differs in escaping and some features).
