import moment from 'moment';
import swal from 'sweetalert';
import api from './api';

export default
{
    setTitle(title)
    {
        let site = document.head.querySelector('meta[name="site"]').content;

        if (title)
        {
            site = title + ' | ' + site;
        }

        document.title = site;
    },

    alert(type, title, message, timer)
    {
        swal({
            title: title,
            text: message || '',
            icon: type,
            closeOnEsc: true,
            closeOnClickOutside: true,
            timer: timer || null
        });
    },

    activate(element)
    {
        let $element = $(element);

        $element.find('.ui.dropdown').dropdown();
    },

    truth(val)
    {
        return val === true || val === 1 || val === '1' || val === 'on' || val === 'yes' || val === 'true' || val === 'si';
    },

    filesize(bytes)
    {
        let units = ['B', 'KB', 'MB', 'GB', 'TB', 'PB'], i;
        for (i = 0; bytes >= 1024; i++)
        {
            bytes /= 1024;
        }

        return (Math.round(bytes * 100) / 100) + ' ' + units[i];
    },

    moment(time)
    {
        if (typeof time === 'object')
        {
            return moment(time.date);
        }

        return moment(time);
    },

    pagination(data)
    {
        return {
            from: data.from,
            to: data.to,
            total: data.total,

            current_page: data.current_page,
            last_page: data.last_page,
            per_page: data.per_page,
        };
    },

    deleteConfirm(url, item, complete)
    {
        item = item || 'item';

        swal({
            title: 'Are you sure?',
            text: `Once deleted, you will not be able to recover this ${item}!`,
            icon: 'warning',
            buttons: true,
            dangerMode: true,
        }).then((d) =>
        {
            if (d)
            {
                api.delete(url, {}, () =>
                {
                    swal(`Poof! The ${item} has been deleted!`, {icon: 'success', timer: 2000});

                    if (complete)
                    {
                        complete();
                    }
                });
            }
        });
    }
}