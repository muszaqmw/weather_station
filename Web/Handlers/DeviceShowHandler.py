from pony import orm
from Database import Device
from flask import render_template
from Utils import random_chars


@orm.db_session
def handle(request):
    devices = orm.select(d for d in Device.Device)

    return render_template('devices.html', devices=devices, new_token=random_chars(12))
