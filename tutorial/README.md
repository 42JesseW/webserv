# Turials I've followed

These individual .c files contain small web related projects. Each individual
`web_*.c file` can be compiled together with `custom.c` to create a simple program.

For example:
```bash
> gcc custom.c web_client.c && ./a.out $(dig +short google.com)
```

Will send a request to www.google.com and write to STDOUT the data retrieved.
