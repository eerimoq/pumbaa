#!/usr/bin/env python3

import socket
import ssl

SERVER_IP = '192.168.0.4'
SERVER_PORT = 10023

CERTIFICATE = """
-----BEGIN CERTIFICATE-----
MIIDBjCCAe4CCQDxnqw02a38fDANBgkqhkiG9w0BAQsFADBFMQswCQYDVQQGEwJB
VTETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50ZXJuZXQgV2lkZ2l0
cyBQdHkgTHRkMB4XDTE2MTIxODA4Mzg1MloXDTE3MTIxODA4Mzg1MlowRTELMAkG
A1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoMGEludGVybmV0
IFdpZGdpdHMgUHR5IEx0ZDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
ALYNY/GZ2QNYdVFv39vMxFkXp7AogTYfVSqFoJwPneaXAt24aiKDwHO3OO3W8nYe
AwBZ9Jl1zQKA5wafc5uaRUH4Nuq1INiQZQ6gYL74KaBjNoX8TgLBOjci2Kea8HAZ
Ao/KY88UE1CG7sErJjuZ5SyKYGzdFuPek+jny8OxzqpPqP7ORrDqYKbFOOpUUhHZ
kPeVMhbWR2OSn052RtO63rtDJ49LltFy+8KXV6SaEu7Zuyk4WaBObSXLlMFDZKp9
ZlXYOKSZ2xGcKtgnMOTous1j/XxGTd0Xlbr2TZi/cfq7xFqfWRjfWiTX9w5d/KWd
h1jv9lrNsRfqK86FKFW2SOECAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAD33b4qXN
BA13KsOSba7zCla8Z5dKk8JOwTp0wM7xqTWzR/eN7xkaQ6NFZavsbGACtHbGs2vU
DVRsTzTzn/0RVoCaEqYsELwXoaz7sUBZoc39+kwbC5BCvZlHLPRBkEy9nAehqIg6
bbzbVObZ1gqI1GWXtVONv65n1W8hjhTa2L841EMCVKKeR5sg1Pts0W7F3e9K2r7j
NumMWFa8RxDPvHwfE0zXzJYbXLAbDClvdjNboAq/jZu6f1O71yAJkPNCZKHJKFKO
2p4hsmEigByb2jr0BB6Bsmdcf1RyRwIemJ5fXauUsvJydYcvklo0xcpqQrTkhIzb
UoNMyyG9ikCJjA==
-----END CERTIFICATE-----
"""

# The certificate must be in a file.
with open('server.crt', 'w') as f:
    f.write(CERTIFICATE)

print('Connecting to {}:{}.'.format(SERVER_IP, SERVER_PORT))

sock = socket.socket()
sock.connect((SERVER_IP, SERVER_PORT))

print('Connected. Wrapping the TCP socket in SSL.')

context = ssl.SSLContext(ssl.PROTOCOL_TLS)
context.load_verify_locations("server.crt")

ssl_sock = context.wrap_socket(sock)

print('Server hostname:', ssl_sock.get_server_hostname())
print('Cipher:', ssl_sock.cipher())

print('write: Hello!')
ssl_sock.send(b"Hello!")
print('read:', ssl_sock.recv(8))

ssl_sock.close()
sock.close()
