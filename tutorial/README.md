# Turials I've followed

These individual .c files contain small web related projects. Each individual
`web_*.c file` can be compiled together with `custom.c` to create a simple program.

For example:
```bash
> gcc custom.c web_client.c && ./a.out $(dig +short google.com)
```

Will send a request to www.google.com and write to STDOUT the data retrieved.


## Docker

In the `docker` directory you'll find the default nginx `default.conf` file. Using the Dockerfile
you can more easily check the behaviours of nginx when certain configuration settings are set.

**Some handy information**
* `/etc/nginx/` is the default location for nginx configuration files
* `/usr/share/nginx` is the default location for nginx to serve files from
