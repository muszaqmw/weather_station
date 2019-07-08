from flask import render_template


def handle(request):
    return render_template('dashboard.html')
