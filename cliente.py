import xmlrpc.client

with xmlrpc.client.ServerProxy("http://localhost:8000/") as proxy:
    print("170 é par: %s" % str(proxy.is_even(170)))
    print("1001 é par: %s" % str(proxy.is_even(1001)))