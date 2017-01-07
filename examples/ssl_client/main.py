#!/usr/bin/env python3

import socket
import ssl

SERVER_IP = '192.168.0.4'
SERVER_PORT = 10023

CERTIFICATE = """
-----BEGIN CERTIFICATE-----
MIIDCjCCAfICCQCoXfSQydXTczANBgkqhkiG9w0BAQsFADBHMQswCQYDVQQGEwJT
RTETMBEGA1UECAwKU29tZS1TdGF0ZTEOMAwGA1UECgwFU2ltYmExEzARBgNVBAMM
CmZvb2Jhci5vcmcwHhcNMTcwMTA3MTgxOTQ3WhcNMTgwMTA3MTgxOTQ3WjBHMQsw
CQYDVQQGEwJTRTETMBEGA1UECAwKU29tZS1TdGF0ZTEOMAwGA1UECgwFU2ltYmEx
EzARBgNVBAMMCmZvb2Jhci5vcmcwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK
AoIBAQDugoc9amynDB/ODJzh2aqB6cnubSJEPlgB87jNG0akcbmUFt6BhPhMPSoP
esHWl5OWscoga0cKrPURmMVVhfaeZLQGmrv5N4/liVlwae1n0gUEruX4d6MqSSDW
3C/WKjCn647udZwyzCvyrvPOq0qAzaxR4EFRdwjSEPO5sDw2zxeTjGW2WxaH9PEu
C8vaNqTsLYl3YBkR3zVCbuQXTQhStsv3gT4Yhz2wJpY0yyWyDiaBkGKpdxJQiNAd
x5JKSqtRshlYZM3+cdKLywNoYUnezp6Wm4mzz09TCFv+esJ0h7/6pMdVjhxLsAg5
ZbZyrtNIapN07AjIJS4qjkJ/HUC3AgMBAAEwDQYJKoZIhvcNAQELBQADggEBAGHS
U5AvDESzTNoak5HHx166bp5+bvECvJg45GMDGhqKeFoOlR34u2b+wyTm/piz3nYJ
12kn+BbG/PwGfndL0kZYRz46fY8Rf+MxCFfcglweDJhA6ULNpera11OC35Q/lKn5
M6w6eQkZMB4VqwigvDGHGpXRTIhJHHoR2VFBFGoPTLrXilChUpiXi9DmuYXJ/19x
sxOVwvvO/m/6g68G+uZYUoCsQsKllM2fgdNLTzbYvnFtsq5QnZS8m7CoZgEy2c3m
VrrPsfwmyhwejDawjz2epSLNXaaDeSz4g1cQes+oehaA1IwFfKhb9tdiWUm48VuU
oaFZ8e1HofYUy+65pws=
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

ssl_sock = context.wrap_socket(sock, server_hostname='foobar.org')

print('Server hostname:', ssl_sock.get_server_hostname())
print('Cipher:', ssl_sock.cipher())

print('write: Hello!')
ssl_sock.send(b"Hello!")
print('read:', ssl_sock.recv(8))

ssl_sock.close()
sock.close()
