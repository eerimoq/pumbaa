#!/usr/bin/env python3

import socket
import ssl
import select


SERVER_IP = '192.168.0.7'
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

KEY = """
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAtg1j8ZnZA1h1UW/f28zEWRensCiBNh9VKoWgnA+d5pcC3bhq
IoPAc7c47dbydh4DAFn0mXXNAoDnBp9zm5pFQfg26rUg2JBlDqBgvvgpoGM2hfxO
AsE6NyLYp5rwcBkCj8pjzxQTUIbuwSsmO5nlLIpgbN0W496T6OfLw7HOqk+o/s5G
sOpgpsU46lRSEdmQ95UyFtZHY5KfTnZG07reu0Mnj0uW0XL7wpdXpJoS7tm7KThZ
oE5tJcuUwUNkqn1mVdg4pJnbEZwq2Ccw5Oi6zWP9fEZN3ReVuvZNmL9x+rvEWp9Z
GN9aJNf3Dl38pZ2HWO/2Ws2xF+orzoUoVbZI4QIDAQABAoIBAQCSDSIONK/eODw+
bjM4ogkvtIOKFqS6p95qc7PLH9g8ow/gks9LC0n9S5O3c4cm7aLYyFIsP06OYJ13
ObrErbo8V4avO0qzkvwQBOg4rNywWIfoa+al3SD4FNOdevbD57yfYO47tk5cfzPR
8u0ESc2KsyTykViIzQr6geN+BwrWpB+vP2DLdoDlhVwtMnlbViRFjsFOhxpfrgA6
dSWRrfD7qv6hA62BEttk+L30vN/uLLLdLPLHtqiNwp3ri7AkLf/WEpACvHwEvCdg
2dGNrQHLUpF0WPuIEV6/R0ps3wXcfLJB9ZWX68U2lGb+w1ue7o8VEiTKfGVTK6nD
xSyU0FEBAoGBAO8GQoBlTZj771YIZ0PxWn62Gk+F1E5paUKLRsoaeWzLyxEawnwS
crShxWUsK24N/b6JsSwRxJe7uSplKP9VBvnjxecUc+9wXpIjd6pRSjBcSqliCMDO
i72SziCu7SOkUU/wfwzmlRgWveRAixOyRUTI/rn+9X69t6uojBxsoQsxAoGBAML7
T7H3xVPgWFgbZuaDZiELZ3uGt6ggsEPB9OhUmuQhklwSpY6CSsHlsc/QqQLOk+33
yksLn0VRUly8Qk3Yn9W1jzvRAt1N1TzU6bV30bUUFW8y/NjHr/46y6P+sqOWVsA9
73kWLKYQS3eXruWH9P6gUNrWoF09tCJsATIS+0yxAoGBAIemLM2DMFOW/s/7eudz
LKFmpW3mDIPnjMYoFqeR+MFGstrlLeSN760VddoiBA+0PwIIUJUrDD/0WHR37plh
XtEoc+Ldl6IWSoVzPXFaKtlhNzoDmeYaEfInQ3YG8CvfEApm+SOQJKMEHQZRntwx
ut/7lZxm1ln0Py1bPRDQkWCxAoGBAK0FrdbTc7vWnQ6yYkGz4HD6Wb338gmz2DaI
avev/DLsx6AEu/0WCZi+Z2duVkwzXHoTJOQrUA6G93trY7sIu+08y/nco5oWc18j
4LAzv1fclpiyukT0DmyfaA4C+irIQfsd+t+MmdpTI2TgqUbtuq4pPRa01mFnW+4f
wX/Z8D5BAoGALEvKgnYKO74qcrgG58FylvaXwyv9oaDEioRZ2fLHbcTU6M2yTdrA
YlbSZl9moMeV18cHr55PzL/qxFQuoSQ26kA7Fag1MYLvakyL3Y0QnhTI+/JvqFLZ
CTsgcsS/vH3Kvg+o/FsQmz/T0Pk+Q0Zb94qnlK30gdY6AQFVb2A71p0=
-----END RSA PRIVATE KEY-----
"""


def recvall(sock, length):
    buf = b''
    while len(buf) < length:
        byte = sock.recv(1)
        if not byte:
            break
        buf += byte
    return buf


# The certificate must be in a file.
with open('server.crt', 'w') as f:
    f.write(CERTIFICATE)

# The key must be in a file.
with open('server.key', 'w') as f:
    f.write(KEY)

context = ssl.SSLContext(ssl.PROTOCOL_TLS)
context.load_cert_chain("server.crt",
                        keyfile="server.key")

listener_sock = socket.socket()
listener_sock.bind((SERVER_IP, SERVER_PORT))
listener_sock.listen(1)

# To give an example of how to use the select module.
poll = select.poll()

while True:
    print('Waiting for a client to connect to {}:{}.'.format(
        SERVER_IP, SERVER_PORT))
    client_sock, fromaddr = listener_sock.accept()
    print('Client connected.')
    ssl_client_sock = context.wrap_socket(client_sock, server_side=True)
    poll.register(client_sock)
    try:
        print('polling')
        poll.poll()
        print('poll returned')
        print('read:', recvall(ssl_client_sock, 6))
        print('write: Goodbye!')
        ssl_client_sock.send(b'Goodbye!')
    finally:
        poll.unregister(client_sock)
        ssl_client_sock.close()
        client_sock.close()
