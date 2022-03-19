import sys
import pytest
import pathlib
import requests
import threading
from subprocess import Popen

CWD = pathlib.Path(__file__).parent.absolute()

"""
pytest explanation:

pytest -rP gives extra test summary: (P)assed with output

pytest discovers functions and classed with tests based on the name.
Functions start with either test_* or end with *_test and classes are
the same but then in PascalCase for Test* and *Test.

a function uses a fixture when its name is registered as an argument
to the test function. Each fixture can return something which the test
function can then use.

With parametrize you can create more arguments which can be passed to
the individual test functions. The `indirect` option allows specfied fixtures
to have access to the specified argument(s).
"""


class WebservProcessWrapper:

    TMP_CONFIG_FILE = CWD / 'default.conf'

    def __init__(self, config_file_data: str):
        self._config_file_data = config_file_data

    def __enter__(self):
        # create the config file for the webserv program
        path = pathlib.Path(WebservProcessWrapper.TMP_CONFIG_FILE)
        with open(path, mode='w+') as f:
            f.write(self._config_file_data)

    def __exit__(self, exc_type, exc_val, exc_tb):
        # remove the config file for the webserv program
        pathlib.Path(WebservProcessWrapper.TMP_CONFIG_FILE).unlink(missing_ok=True)

        # kill the process if created using create_webserv_process
        if hasattr(self, '_popen'):
            self._popen.kill()

    def create_webserv_process(self):
        self._popen = Popen([
            str((CWD / '../../webserv').absolute()),                # path to webserv program
            str(WebservProcessWrapper.TMP_CONFIG_FILE.absolute())   # path to config file that was created
        ])
        self._popen.wait()
        if self._popen.returncode != 0:
            print("failed to start webserv", file=sys.stderr)
            raise Exception("create_webserv_process error!")


@pytest.fixture
def make_webserv():
    """
    Make sure that for each test the `webserv` executable is compiled and
    ready to be run. Return the exit code of the process to the test case.
    :return:
    """
    return Popen(['make', '-C', CWD / '../..']).wait()


@pytest.fixture(name='config')
def webserv(request):
    """
    Start the webserver in a background thread (daemon). if the tests for some reason
    raise an exception, the yield will still allow the context manager to exit
    :return:
    """
    wrapper = WebservProcessWrapper(request.param)
    with wrapper:
        thread = threading.Thread(target=wrapper.create_webserv_process)
        thread.daemon = True
        thread.start()
        yield wrapper
    thread.join()

@pytest.mark.parametrize(
    'config, port',
    [(
            """
            http {
                server {
                    listen          *:8082
                    server_name     example.org www.example.org
                    client_max_body_size 10
                    error_page 400 /full/path/to/file.html
                    location / {
                        allowed_methods GET POST DELETE
                        root            /data/w3
                        autoindex       on
                        index           index.html index.htm
                        cgi_extensions  .php .py
                        upload_path     /data/upload
                        return          301 www.google.com
                    }
                }
            }
            """,
            8082
    )], indirect=['config'])
class TestStatusCode2xx:

    def test_statuscode_200(self, config, port, make_webserv):
        assert make_webserv == 0


class TestStatusCode3xx:

    def test_statuscode_300(self):
        assert 1


class TestStatusCode4xx:

    def test_statuscode_400(self):
        assert 1


class TestStatusCode5xx:

    def test_statuscode_500(self):
        assert 1
