import ipaddress
import webbrowser
import socket

# a_website = ""
_, _, ipadd = socket.gethostbyname_ex(socket.gethostname())
a_website = "{}/index.html".format(ipadd[0])


# Open url in a new window of the default browser, if possible
#webbrowser.open_new(a_website)

# Open url in a new page (“tab”) of the default browser, if possible
#webbrowser.open_new_tab(a_website)
for i in range(100):
    print(i)
    webbrowser.get('chrome').open_new_tab(a_website)

