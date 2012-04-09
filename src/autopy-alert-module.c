#include "autopy-alert-module.h"
#include "alert.h"
#include <time.h>

/* Syntax: alert(msg, title="AutoPy Alert", default_button="OK",
                 [cancel_button]) */
/* Arguments: |msg| => string,
              |title| => string,
              |default_button| => string,
              |cancel_button| => string */
/* Description: Displays alert with the given attributes. If |cancel_button| is
                not given, only the default button is displayed. Returns True
                if the default button was pressed, or False if cancelled. Note
                that the arguments are keywords, and can be passed as named
                parameters (e.g., `alert(msg="bar", title="foo")`).

                Due to limitations in the Win32 API, Windows currently replaces
                |default_button| with `"OK"` and |cancel_button| (if given)
                with `"Cancel"`. This may be fixed in a later release. */
static PyObject *alert_alert(PyObject *self, PyObject *args, PyObject *kwds);

static PyMethodDef AlertMethods[] = {
	{"alert", (PyCFunction)alert_alert, METH_VARARGS | METH_KEYWORDS,
	 "alert(msg, title='AutoPy Alert', default_button='OK', "
	 "[cancel_button]) => Boolean\n"
	 "Displays alert with the given attributes.\n"
	 "Returns True if the default button was pressed, or False if cancelled."},
	{NULL, NULL, 0, NULL} /* Sentinel */
};

PyMODINIT_FUNC initalert(void)
{
	Py_InitModule3("alert", AlertMethods,
	               "autopy module for displaying alerts");
}

static PyObject *alert_alert(PyObject *self, PyObject *args, PyObject *kwds)
{
	char *msg = NULL, *title = "AutoPy Alert";
	char *default_button = "OK", *cancel_button = NULL;
        time_t timeout = -1;

	static char *kwdlist[] = {"title", "msg", "default_button",
	                          "cancel_button", "timeout", NULL};
	int button;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|szzk", kwdlist,
	                                 &msg,
	                                 &title,
	                                 &default_button,
	                                 &cancel_button,
                                         &timeout)) {
		return NULL;
	}

	button = showAlert(title, msg, default_button, cancel_button, timeout);
	if (button == 0) {
		Py_RETURN_TRUE;
	} else if (button == 1) {
		Py_RETURN_FALSE;
	} else if (button == -2) {
                Py_RETURN_NONE;
        }
        else {
		PyErr_SetString(PyExc_OSError, "Could not display alert");
		return NULL;
	}
}
