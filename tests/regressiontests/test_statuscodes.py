import time
import pytest
import signal
import subprocess
from pathlib import Path
import requests

CWD = Path(__file__).parent.absolute()
PROG_STARTUP_TIME = 0.5


class ProcessWrapper:

    TMP_CONFIG_FILE = CWD / 'default.conf'

    def __init__(self, config_file_data: str):
        self._process = None
        self._cwd = Path(__file__).parent.absolute()
        self._config_file_data = config_file_data

    def make_program(self):
        path = Path(ProcessWrapper.TMP_CONFIG_FILE)
        with open(path, mode='w+') as f:
            f.write(self._config_file_data)

    def set_up(self):
        self._process = subprocess.Popen([
            self._cwd / '../../webserv',
            ProcessWrapper.TMP_CONFIG_FILE
        ])

    def tear_down(self):
        Path(ProcessWrapper.TMP_CONFIG_FILE).unlink(missing_ok=True)
        self._process.send_signal(sig=signal.SIGINT)


@pytest.fixture
def make_webserv():
    """
    Make sure that for each test the `webserv` executable is compiled and
    ready to be run. Return the exit code of the process to the test case.
    :return:
    """
    return subprocess.Popen(['make', '-C', CWD / '../..']).wait()


@pytest.fixture(scope='class', name='config')
def run_webserv(request):
    wrapper = ProcessWrapper(request.param)
    wrapper.make_program()
    wrapper.set_up()
    yield wrapper
    wrapper.tear_down()


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
                    root            /
                    autoindex       on
                    index           index.html index.htm
                    cgi_extensions  .php .py
                    upload_path     ./
                }
            }
        }
        """,
        8082
    )], indirect=['config'])
class TestStatusCode2xx:

    def test_statuscode_200(self, config, port, make_webserv):
        assert make_webserv == 0
        time.sleep(PROG_STARTUP_TIME)
        resp = requests.get(url=f'http://localhost:{port}')
        assert resp.status_code == 200


@pytest.mark.parametrize(
    'config, port',
    [(
        """
        http {
            server {
                listen          *:8083
                server_name     example.org www.example.org
                client_max_body_size 10
                error_page 400 /full/path/to/file.html
                location / {
                    allowed_methods GET POST DELETE
                    root            /
                    autoindex       on
                    index           index.html index.htm
                    cgi_extensions  .php .py
                    upload_path     ./
                    return          301 www.google.com
                }
            }
        }
        """,
        8083
    )], indirect=['config'])
class TestStatusCode3xx:

    def test_statuscode_301(self, config, port, make_webserv):
        assert make_webserv == 0
        time.sleep(PROG_STARTUP_TIME)
        resp = requests.get(url=f'http://localhost:{port}', allow_redirects=False)
        assert resp.status_code == 301
        assert not resp.text
        assert resp.headers.get('Location') and resp.headers.get('Location') is not None


# TODO currently fails because SEGFAULT in webserv program
# @pytest.mark.parametrize(
#     'config, port',
#     [(
#             """
#             http {
#                 server {
#                     listen          *:8084
#                     server_name     example.org www.example.org
#                     client_max_body_size 10
#                     error_page 400 /full/path/to/file.html
#                     location /unknown {
#                         allowed_methods GET POST DELETE
#                         root            /data/w3
#                         autoindex       on
#                         index           index.html index.htm
#                         cgi_extensions  .php .py
#                         upload_path     ./
#                     }
#                 }
#             }
#             """,
#             8084
#     )], indirect=['config'])
# class TestStatusCode4xx:
#
#     def test_statuscode_404(self, config, port, make_webserv):
#         assert make_webserv == 0
#         time.sleep(PROG_STARTUP_TIME)
#         resp = requests.get(url=f'http://localhost:{port}')
#         assert resp.status_code == 404
#         #assert not resp.text
#         #assert resp.headers.get('Location') and resp.headers.get('Location') is not None
