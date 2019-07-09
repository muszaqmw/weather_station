from pony import orm
from flask import redirect, url_for
from Database import Device

@orm.db_session
def handle(request):
    if request.method == 'POST':
        description = request.form['description']
        token = request.form['token']

        if description and token:
            Device.Device(token=token, description=description)
            orm.commit()

    return redirect(url_for('showDevices'))