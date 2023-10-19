def figurer(fighandle):
    import io
    bytearray_buffer = io.BytesIO()
    fighandle.savefig(bytearray_buffer, format="png")
    global figure_image_data
    figure_image_data = bytearray(bytearray_buffer.getvalue())
    bytearray_buffer.close()

